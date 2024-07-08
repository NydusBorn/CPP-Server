#include "ConnectorBase.cpp"
#include "MapDecryptor.cpp"
#include "fileChecksum.h"

class ConnectorBotnet : public ConnectorBase{
protected:
    std::vector<char> decrypted;
    std::unordered_set<int> incompleteChunks;
    std::unordered_map<std::string, int> orderList;
    std::mutex mutex;

    [[nodiscard]] std::string mainreq(const std::string &req_enc){
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
        std::scoped_lock lock(mutex);
        uniqueIDs.emplace(req_json["UUID"]);
        uniqueAddresses.emplace(req_json["PublicIP"]);
        expiringTimes.emplace_back(std::chrono::system_clock::now() + std::chrono::seconds(1));
        if (incompleteChunks.empty()){
            return "";
        }
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, incompleteChunks.size() - 1);
        auto id = dist(rng);
        std::string orderID = "";
        dist = std::uniform_int_distribution<std::mt19937::result_type>('A', 'Z');
        for (int i = 0; i < 32; i++) {
            orderID += static_cast<char>(dist(rng));
        }
        int counter = 0;
        int where = 0;
        for (auto it : incompleteChunks){
            if (counter == id){
                orderList[orderID] = it;
                where = it;
                break;
            }
            counter++;
        }
        std::map<std::string, std::string> data = {
                {"ID", orderID},
                {"task", checksum::fileChecksum[where]}
        };
        std::string message = nlohmann::json(data).dump();
        return aes.encrypt(message);
    }

    void mainres(const std::string &req_enc){
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
        std::scoped_lock lock(mutex);
        uniqueIDs.emplace(req_json["UUID"]);
        uniqueAddresses.emplace(req_json["PublicIP"]);
        expiringTimes.emplace_back(std::chrono::system_clock::now() + std::chrono::seconds(1));
        if (orderList.contains(req_json["ID"]) && incompleteChunks.contains(orderList[req_json["ID"]])){
            incompleteChunks.erase(orderList[req_json["ID"]]);
            std::vector<int> bytes = req_json["comp"];
            auto insert = orderList[req_json["ID"]];
            orderList.erase(req_json["ID"]);
            for (int i = 0; i < 3; i++){
                decrypted[insert*3 + i] = static_cast<char>(bytes[i]);
            }
        }
    }

public:
    ConnectorBotnet(uint16_t port, int threads) : decrypted(3*checksum::fileChecksum.size()) {
        role = Role::Server;
        this->port = port;
        this->connection = std::make_unique<rpc::server>(port);
        rsaEncryptor = std::make_unique<RSAEncryptor>(4096);

        for (int i = 0; i < checksum::fileChecksum.size()*3; i++){
            decrypted.emplace_back(0);
        }
        for (int i = 0; i < checksum::fileChecksum.size(); i++){
            incompleteChunks.insert(i);
        }

        connection->bind("key", [this](){
            return keyreq();
        });
        connection->bind("request", [this](const std::string &req_enc){
            return mainreq(req_enc);
        });
        connection->bind("submit", [this](const std::string &req_enc){
            mainres(req_enc);
        });
        connection->async_run(threads);
        httplib::Client cli("http://api.ipify.org");
        PublicIP = cli.Get("/")->body;
    }

    ConnectorBotnet(const std::string& address, uint16_t port){
        role = Role::Client;
        this->port = port;
        this->address = address;

        std::string ckey;
        std::string iv;
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

    [[nodiscard]] bool makeRequest(){
        if (getRole() == Role::Server) {
            throw incorrectRole("Server is not allowed to make requests");
        }
        std::string enc_data = "";
        enc_data += rsaEncryptor->encrypt(keystr);
        auto jdata = nlohmann::json();
        jdata["UUID"] = UUID;
        jdata["PublicIP"] = PublicIP;
        enc_data += aesEncryptor->encrypt(jdata.dump());

        rpc::client cl(address, port);
        std::string enc_reply = cl.call("request", enc_data).as<std::string>();
        if (enc_reply.empty()) {
            return false;
        }

        std::string reply = aesEncryptor->decrypt(enc_reply);
        auto tjdata = nlohmann::json::parse(reply);
        jdata["ID"] = tjdata["ID"];
        std::string task = tjdata["task"];
        auto complete = MapDecryptor::map_block(task);

        std::vector<int> bytes;
        for (int i = 0; i < 3; i++){
            bytes.push_back(complete[i]);
        }
        jdata["comp"] = bytes;
        enc_data = rsaEncryptor->encrypt(keystr);
        enc_data += aesEncryptor->encrypt(jdata.dump());
        cl.call("submit", enc_data);
        return true;
    }

    [[nodiscard]] uint64_t getIncompleteCount(){
        if (getRole() == Role::Client) {
            throw incorrectRole("Client is not allowed to get incomplete chunks");
        }
        return incompleteChunks.size();
    }

    [[nodiscard]] std::vector<char> getDecrypted(){
        if (getRole() == Role::Client) {
            throw incorrectRole("Client is not allowed to get decrypted data");
        }
        return decrypted;
    }
};