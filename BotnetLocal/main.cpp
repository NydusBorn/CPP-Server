#include "../lib/ConnectorBotnet.cpp"
#include "iostream"
int main() {
    ConnectorBotnet botnet(4000, 12);
    int bot_count = std::thread::hardware_concurrency();
    std::vector<std::jthread> threads;
    int counter = 0;
    for (int i = 0; i < bot_count; ++i) {
        threads.emplace_back([&counter](){
            ConnectorBotnet bot("127.0.0.1", 4000);
            while (true) {
                if (!bot.makeRequest()) break;
                std::cout << "Completed chunk " << counter + 1 << " of " << checksum::fileChecksum.size() << " [" << std::fixed << std::setprecision(2) << static_cast<double>(counter + 1) / checksum::fileChecksum.size() * 100.0 << "%]" << std::endl;
                counter += 1;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
    auto result = botnet.getDecrypted();
    auto out = std::ofstream ("output.jpg");
    out.write(result.data(), result.size());
    out.close();
    return 0;
}