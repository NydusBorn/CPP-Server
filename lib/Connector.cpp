#include "nlohmann/json.hpp"
#include "iostream"
#include "rpc/server.h"
#include "rpc/client.h"
#include "RSAEncryptor.cpp"
#include "AESEncryptor.cpp"
#include "random"

#pragma once


class Connector {
public:
    enum class Role {
        Server,
        Client
    };
    class incorrectRole : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };
private:
    Role role;
    uint16_t port;
    std::string address;
    std::optional<rpc::server> connection;
    std::unique_ptr<RSAEncryptor> rsaEncryptor;
    std::unique_ptr<AESEncryptor> aesEncryptor;
    std::string keystr;
public:
    [[nodiscard]] std::string keyreq() const {
        if (role == Role::Client) {
            throw incorrectRole("Client is not allowed to receive requests");
        }
        return rsaEncryptor->getPublicKey();
    };

    [[nodiscard]]std::string mainreq(const std::string &req_enc) const {
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

        std::string message = req_json["message"];
        if (!message.contains("hello")) {
            return aes.encrypt("fail");
        } else {
            return aes.encrypt("success " + message);
        }
    };

    Connector() {
        role = Role::Server;
        rsaEncryptor = std::make_unique<RSAEncryptor>(4096);
    }

    Connector(std::string address, uint16_t port) : port(port), address(std::move(address)) {
        role = Role::Client;

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
        }
        aesEncryptor = std::make_unique<AESEncryptor>(ckey, iv);
        rpc::client cl(address, port);
        rsaEncryptor = std::make_unique<RSAEncryptor>(cl.call("key").as<std::string>(), true);
    }

    [[nodiscard]] constexpr Role getRole() const {
        return role;
    }

    [[nodiscard]] constexpr int getPort() const {
        return this->port;
    }

    [[nodiscard]] constexpr std::string getAddress() const {
        return this->address;
    }

    [[nodiscard]] constexpr std::string makeRequest(const std::string &req_json) {
        if (getRole() == Role::Server) {
            throw incorrectRole("Server is not allowed to make requests");
        }
        std::string enc_data = "";
        enc_data += rsaEncryptor->encrypt(keystr);
        enc_data += aesEncryptor->encrypt(req_json);

        rpc::client cl(address, port);
        std::string enc_reply = cl.call("request", enc_data).as<std::string>();

        return aesEncryptor->decrypt(enc_reply);
    }
};