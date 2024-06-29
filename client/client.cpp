#include <iostream>
#include <nlohmann/json.hpp>
#include "../lib/Connector.cpp"

int main() {
    Connector client("127.0.0.1", 4000);

    std::cout << (client.getRole() == Connector::Role::Client ? "Client" : "Server") << std::endl;

    std::map<std::string, std::string> data2 = {
        {"message", "hello"}
    };
    nlohmann::json data = data2;
    std::cout << client.makeRequest(data.dump()) << std::endl;

    return 0;
}
