#include <asio.hpp>
#include <iostream>
// g++ -std=c++11 -DASIO_STANDALONE -I C:\programs\vcpkg\installed\x64-windows\include client.cpp -o main.exe -lws2_32

int main() {
    try {
        // Создание I/O контекста
        asio::io_context io_context;

        // Создание и резолвинг TCP endpoint. Предполагается, что сервер слушает на порту 4000.
        asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "4000");

        // Создание и подключение сокета
        asio::ip::tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        std::cout << "Connected to the server!" << std::endl;

        // Для демонстрации: чтение сообщения от сервера
        for (;;) {
            char buf[512];
            std::error_code error;

            size_t len = socket.read_some(asio::buffer(buf), error);

            if (error == asio::error::eof) {
                std::cout << "Connection closed by server" << std::endl;
                break; // Соединение закрыто сервером
            } else if (error) {
                throw asio::system_error(error); // Некоторые другие ошибки.
            }

            std::cout.write(buf, len);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
