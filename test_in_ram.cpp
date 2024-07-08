#include <iostream>
#include <unordered_map>
#include <array>
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>

// Function to compute MD5 hash
std::string md5(const std::string& str) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), digest);

    std::ostringstream oss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    return oss.str();
}

// Function to generate all 3-byte combinations and their MD5 hashes at compile time
constexpr auto generate3ByteMD5Map() {
    std::array<std::pair<std::string, std::string>, 256 * 256 * 256> map{};
    size_t index = 0;

    for (unsigned char i = 0; i < 256; ++i) {
        for (unsigned char j = 0; j < 256; ++j) {
            for (unsigned char k = 0; k < 256; ++k) {
                std::string combination;
                combination += static_cast<char>(i);
                combination += static_cast<char>(j);
                combination += static_cast<char>(k);
                map[index++] = {md5(combination), combination};
            }
        }
    }

    return map;
}

constexpr auto md5Map = generate3ByteMD5Map();

int main() {
    // Example output of some values from the map
    int count = 0;
    for (const auto& pair : md5Map) {
        std::cout << "MD5: " << pair.first << " - Combination: "
                  << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(pair.second[0])
                  << std::setw(2) << std::setfill('0') << static_cast<int>(pair.second[1])
                  << std::setw(2) << std::setfill('0') << static_cast<int>(pair.second[2])
                  << std::dec << std::endl;
        if (++count >= 10) break; // Limit output to the first 10 entries
    }

    return 0;
}
