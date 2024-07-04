#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>
#include <thread>
#include <vector>
#include <atomic>

class BruteForcer {
public:
    BruteForcer() {}

    // Вычисляет MD5-хэш для заданных данных
    std::string calculate_md5(const char* data, std::size_t length) {
        unsigned char md[MD5_DIGEST_LENGTH];
        MD5((unsigned char*)data, length, md);

        std::ostringstream ss;
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)md[i];
        }
        return ss.str();
    }

    // Перебирает возможные значения блоков и ищет совпадение с заданной контрольной суммой
    std::vector<char> brute_force_block(const std::string& checksum) {
        std::atomic<bool> found(false);  // Переменная для отслеживания найденного блока
        int num_threads = 2; //std::thread::hardware_concurrency();
        if (num_threads == 0) {
            num_threads = 2;  // Значение по умолчанию, если количество потоков не определено
        }
        std::vector<std::thread> threads;
        std::vector<char> result(3);  // Результат в виде вектора

        int range_size = 256 / num_threads;  // Диапазон значений для каждого потока

        for (int t = 0; t < num_threads; ++t) {
            int start = t * range_size;
            int end = (t == num_threads - 1) ? 256 : (t + 1) * range_size;
            threads.emplace_back([this, &checksum, &result, start, end, &found]() {
                if (this->brute_force_block_range(checksum, result, start, end)) {
                    found.store(true);  // Обновляем переменную, если блок найден
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();  // Ожидаем завершения всех потоков
        }

        if (found.load()) {
            return result;
        } else {
            return {};  // Возвращаем пустой вектор, если блок не найден
        }
    }

private:
    bool brute_force_block_range(const std::string& checksum, std::vector<char>& result, int start, int end) {
        char block[3];
        for (int i = start; i < end; ++i) {
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
