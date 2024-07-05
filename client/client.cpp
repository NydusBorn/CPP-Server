#include <iostream>
#include <nlohmann/json.hpp>
#include "../lib/ConnectorSimple.cpp"

int main() {
    ConnectorSimple client("127.0.0.1", 4000);

    std::cout << (client.getRole() == ConnectorSimple::Role::Client ? "Client" : "Server") << std::endl;

    std::map<std::string, std::string> data2 = {
        {"message", "hello"}
    };
    nlohmann::json data = data2;
    std::cout << client.makeRequest(data.dump()) << std::endl;

    return 0;
}
