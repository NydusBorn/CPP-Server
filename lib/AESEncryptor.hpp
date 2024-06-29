#ifndef AESENCRYPTOR_H
#define AESENCRYPTOR_H

#include <openssl/aes.h>
#include <string>
#include <vector>

class AESEncryptor {
private:
    AES_KEY key;
    unsigned char original_ivec[16];  // Оригинальный IV
    unsigned char ivec[16];           // Текущий IV

    void resetIV();

public:
    inline static const int AES_KEY_LENGTH = 256;
    inline static const int AES_BLOCK_SIZEE = 16;

    AESEncryptor(const std::string& ckey, const std::string& ivec_str);

    std::string encrypt(const std::string& plaintext);
    std::string decrypt(const std::string& encrypted_text);
};

#endif // AESENCRYPTOR_H
