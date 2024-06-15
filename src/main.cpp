#include <iostream>
#include <asio.hpp>
#include <thread>
#include <nlohmann/json.hpp>
#include <queue>

struct Data {
    asio::ip::tcp::socket& socket;
    std::string message;
};

std::mutex queue_lock;
std::queue<Data> msg_queue;
std::vector<std::shared_ptr<asio::ip::tcp::socket>> sockets;

void WorkerThread(){
    while(true){
        Data* data = nullptr;

        {
            std::scoped_lock lock(queue_lock);
            if (!msg_queue.empty()){
                data = &msg_queue.front();
                msg_queue.pop();
            }
        }

        if(data != nullptr) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            data->socket.send(asio::buffer("resolved\n"));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void DispatcherThread() {
    while (true) {
        for (auto& socket : sockets) {
            if (socket->available() > 0){
                std::vector<char> msg(512);
                socket->read_some(asio::buffer(msg));
                std::cout << "Received message: " << msg.data();
                {
                    std::scoped_lock lock(queue_lock);
                    Data data = { *socket, msg.data() };
                    msg_queue.emplace(data);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(int argc, char** argv) {
    asio::io_service service;
    using namespace asio::ip;
    tcp::endpoint endpoint(tcp::v4(), 4000);
    tcp::acceptor acceptor(service, endpoint);

    for (int i = 0; i < 8; i++) {
        std::thread t(WorkerThread);
        t.detach();
    }
    {
        std::thread t(DispatcherThread);
        t.detach();
    }

    std::cout << "[Server] Started" << std::endl;
    while (true) {
        auto socket =std::make_shared<tcp::socket>(service);
        std::cout << "[Server] Waiting for connection" << std::endl;

        acceptor.accept(*socket);
        std::cout << "[Server] Accepted a connection from client" << std::endl;

        std::map<std::string, std::string> data = {
                {"message", "Hello from the server"}
        };

        nlohmann::json j = data;
        std::string msg = j.dump();
        socket->send(asio::buffer(msg));

        sockets.emplace_back(std::move(socket));
    }

    return 0;
}
