#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>
#include <thread>
#include <vector>
#include <atomic>

#pragma once

class BruteForcer {
public:
    BruteForcer() = default;

    // Вычисляет MD5-хэш для заданных данных
    static std::string calculate_md5(const char* data, std::size_t length) {
        unsigned char md[MD5_DIGEST_LENGTH];
        MD5((unsigned char*)data, length, md);

        std::ostringstream ss;
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)md[i];
        }
        return ss.str();
    }

    // Перебирает возможные значения блоков и ищет совпадение с заданной контрольной суммой
    static std::vector<char> brute_force_block(const std::string& checksum) {
        std::vector<char> result(3);  // Результат в виде вектора

        if (brute_force_block_range(checksum, result)) {
            return result;
        } else {
            return {};  // Возвращаем пустой вектор, если блок не найден
        }
    }

private:
    static bool brute_force_block_range(const std::string& checksum, std::vector<char>& result) {
        char block[3];
        for (int i = 0; i < 256; ++i) {
            for (int j = 0; j < 256; ++j) {
                for (int k = 0; k < 256; ++k) {
                    block[0] = i;
                    block[1] = j;
                    block[2] = k;
                    if (calculate_md5(block, 3) == checksum) {
                        result[0] = block[0];
                        result[1] = block[1];
                        result[2] = block[2];
                        return true;
                    }
                }
            }
        }
        return false;
    }
};
