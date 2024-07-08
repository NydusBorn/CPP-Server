#include "BruteForcer.cpp"
#include "unordered_map"

#pragma once

class MapDecryptor{
public:
    static std::string md5(const std::string& str) {
        unsigned char digest[MD5_DIGEST_LENGTH];
        MD5(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), digest);

        std::ostringstream oss;
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
        }
        return oss.str();
    }
private:
    static std::unordered_map<std::string, std::string> generate3ByteMD5Map() {
        std::unordered_map<std::string, std::string> map{};

        for (int i = 0; i < 256; ++i) {
            for (int j = 0; j < 256; ++j) {
                for (int k = 0; k < 256; ++k) {
                    std::string combination;
                    combination += static_cast<char>(i);
                    combination += static_cast<char>(j);
                    combination += static_cast<char>(k);
                    map[md5(combination)] =  combination;
                }
            }
        }

        return map;
    }

    static const std::unordered_map<std::string, std::string> md5Map;

public:
    // Перебирает возможные значения блоков и ищет совпадение с заданной контрольной суммой
    static std::vector<char> map_block(const std::string& checksum) {
        std::vector<char> result(3);  // Результат в виде вектора

        if (md5Map.contains(checksum)) {
            result.emplace_back(md5Map.at(checksum)[0]);
            result.emplace_back(md5Map.at(checksum)[1]);
            result.emplace_back(md5Map.at(checksum)[2]);
            return result;
        } else {
            return {};  // Возвращаем пустой вектор, если блок не найден
        }
    }
};
const std::unordered_map<std::string, std::string> MapDecryptor::md5Map = MapDecryptor::generate3ByteMD5Map();