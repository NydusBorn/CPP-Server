#include "nlohmann/json.hpp"
#include "iostream"
#include "rpc/server.h"
#include "rpc/client.h"
#include "RSAEncryptor.cpp"
#include "AESEncryptor.cpp"
#include "random"
#include "httplib.h"
#include "ConnectorBase.cpp"

#pragma once

class ConnectorSimple : public ConnectorBase {
protected:
    [[nodiscard]] std::string mainreq(const std::string &req_enc) override{
        if (role == Role::Client) {
            throw incorrectRole("Client is not allowed to receive requests");
        }
        std::string enc_key = req_enc.substr(0, 512);
        std::string keystr_t = rsaEncryptor->decrypt(enc_key);
        std::string key = keystr_t.substr(0, 32);
        std::string iv = keystr_t.substr(32);
        AESEncryptor aes(key, iv);
        std::string req = aes.decrypt(req_enc.substr(512));
        nlohmann::json req_json = nlohmann::json::parse(req);
        uniqueIDs.emplace(req_json["UUID"]);
        uniqueAddresses.emplace(req_json["PublicIP"]);
        expiringTimes.emplace_back(std::chrono::system_clock::now() + std::chrono::seconds(1));
        std::string message = req_json["message"];
        if (!message.contains("hello")) {
            return aes.encrypt("fail");
        } else {
            return aes.encrypt("success " + message);
        }
    };

public:
    ConnectorSimple(uint16_t port, int threads) {
        role = Role::Server;
        this->port = port;
        this->connection = std::make_unique<rpc::server>(port);
        rsaEncryptor = std::make_unique<RSAEncryptor>(4096);

        connection->bind("key", [this](){
            return keyreq();
        });
        connection->bind("request", [this](const std::string &req_enc){
            return mainreq(req_enc);
        });
        connection->async_run(threads);
        httplib::Client cli("http://api.ipify.org");
        PublicIP = cli.Get("/")->body;
    }

    ConnectorSimple(std::string address, uint16_t port){
        role = Role::Client;
        this->port = port;
        this->address = address;

        std::string ckey, iv;
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);
            for (int i = 0; i < 32; i++) {
                ckey += static_cast<char>(dist(rng));
            }
            for (int i = 0; i < 16; i++) {
                iv += static_cast<char>(dist(rng));
            }
            keystr = ckey + iv;
            dist = std::uniform_int_distribution<std::mt19937::result_type>('A', 'Z');
            for (int i = 0; i < 128; i++) {
                UUID += static_cast<char>(dist(rng));
            }

            httplib::Client cli("http://api.ipify.org");
            PublicIP = cli.Get("/")->body;
        }
        aesEncryptor = std::make_unique<AESEncryptor>(ckey, iv);
        rpc::client cl(address, port);
        rsaEncryptor = std::make_unique<RSAEncryptor>(cl.call("key").as<std::string>(), true);
    }

    [[nodiscard]] std::string makeRequest(const std::string &req_json) override{
        if (getRole() == Role::Server) {
            throw incorrectRole("Server is not allowed to make requests");
        }
        std::string enc_data = "";
        enc_data += rsaEncryptor->encrypt(keystr);
        nlohmann::json jdata = nlohmann::json::parse(req_json);
        jdata["UUID"] = UUID;
        jdata["PublicIP"] = PublicIP;
        enc_data += aesEncryptor->encrypt(jdata.dump());

        rpc::client cl(address, port);
        std::string enc_reply = cl.call("request", enc_data).as<std::string>();

        return aesEncryptor->decrypt(enc_reply);
    }
};