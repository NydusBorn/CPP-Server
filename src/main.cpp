#include <iostream>
#include <asio.hpp>
#include <thread>
#include <nlohmann/json.hpp>

std::vector<char> buff(256);

void SendHandler(const asio::error_code& ec, std::size_t bytes_transferred) {
    if (ec) {
        std::cout << ec.message() << std::endl;
        return;
    }
    std::cout << "Sent a message" << std::endl;
}

void ReadHandler(const asio::error_code& ec, std::size_t bytes_transferred) {
    if (ec) {
        std::cout << ec.message() << std::endl;
        return;
    }
    std::cout << "Received a message" << std::endl;
    std::cout << buff.data() << std::endl;
}

int main(int argc, char** argv) {
    asio::io_service service;
    using namespace asio::ip;
    tcp::endpoint endpoint(tcp::v4(), 4000);
    tcp::acceptor acceptor(service, endpoint);
    tcp::socket socket(service);
    std::cout << "[Server] Waiting for connection" << std::endl;

    acceptor.accept(socket);
    std::cout << "[Server] Accepted a connection from client" << std::endl;


    std::map<std::string, std::string> data = {
            {"message", "Hello from the server"}
    };

    nlohmann::json j = data;
    std::string msg = j.dump();
    socket.async_send(asio::buffer(msg), SendHandler);
    while (true) {
        socket.async_read_some(asio::buffer(buff), ReadHandler);
        service.poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
