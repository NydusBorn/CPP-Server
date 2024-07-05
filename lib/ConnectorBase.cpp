#include "nlohmann/json.hpp"
#include "iostream"
#include "rpc/server.h"
#include "rpc/client.h"
#include "RSAEncryptor.cpp"
#include "AESEncryptor.cpp"
#include "random"
#include "httplib.h"

#pragma once


class ConnectorBase {
public:
    enum class Role {
        Server,
        Client
    };
    class incorrectRole : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

protected:
    Role role;
    uint16_t port;
    std::string address;
    std::unique_ptr<rpc::server> connection;
    std::unique_ptr<RSAEncryptor> rsaEncryptor;
    std::unique_ptr<AESEncryptor> aesEncryptor;
    std::string keystr;

    std::string PublicIP;
    std::string UUID;

    std::unordered_set<std::string> uniqueAddresses;
    std::unordered_set<std::string> uniqueIDs;
    std::vector<std::chrono::time_point<std::chrono::system_clock>> expiringTimes;


    [[nodiscard]] std::string keyreq() const {
        if (role == Role::Client) {
            throw incorrectRole("Client is not allowed to receive requests");
        }
        return rsaEncryptor->getPublicKey();
    };

public:

    ConnectorBase() = default;
    ConnectorBase(uint16_t port, int threads) : port(port), connection(std::make_unique<rpc::server>(port)){
        role = Role::Server;
        rsaEncryptor = std::make_unique<RSAEncryptor>(4096);

        connection->bind("key", [this](){
            return keyreq();
        });
        connection->async_run(threads);
        httplib::Client cli("http://api.ipify.org");
        PublicIP = cli.Get("/")->body;
    }

    ConnectorBase(std::string address, uint16_t port) : port(port), address(std::move(address)) {
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

    [[nodiscard]] constexpr Role getRole() const {
        return role;
    }

    [[nodiscard]] constexpr int getPort() const {
        return this->port;
    }

    [[nodiscard]] constexpr std::string getAddress() const {
        if (getRole() == Role::Server) {
            throw incorrectRole("Server is not capable of getting it's own address, use getPublicIP() instead");
        }
        return this->address;
    }

    [[nodiscard]] constexpr std::string getUUID() const {
        if (getRole() == Role::Server) {
            throw incorrectRole("Server does not have a UUID");
        }
        return this->UUID;
    }

    [[nodiscard]] constexpr std::string getPublicIP() const {
        return this->PublicIP;
    }

    [[nodiscard]] int getUniqueAddresses() {
        if (role == Role::Client) {
            throw incorrectRole("Client is not capable of collecting statistics");
        }
        return this->uniqueAddresses.size();
    }

    [[nodiscard]] int getUniqueIDs() {
        if (role == Role::Client) {
            throw incorrectRole("Client is not capable of collecting statistics");
        }
        return this->uniqueIDs.size();
    }

    [[nodiscard]] constexpr double getRequestsPerSecond() {
        if (role == Role::Client) {
            throw incorrectRole("Client is not capable of collecting statistics");
        }
        std::vector<std::chrono::time_point<std::chrono::system_clock>> unexpired;
        double rps = 0;
        for (auto time : expiringTimes) {
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(time - std::chrono::system_clock::now()).count();
            if (diff > 0) {
                rps += ((double)diff)/1000.0;
                unexpired.emplace_back(time);
            }
        }
        expiringTimes = unexpired;
        return rps;
    }

    virtual ~ConnectorBase() = default;
};