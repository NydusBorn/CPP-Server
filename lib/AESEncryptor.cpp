#include <openssl/aes.h>
#include <string>
#include <vector>
#include <cstring>

#pragma once

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
void AESEncryptor::resetIV() {
    memcpy(ivec, original_ivec, AES_BLOCK_SIZEE);
}

AESEncryptor::AESEncryptor(const std::string& ckey, const std::string& ivec_str) {
    std::string key_str = ckey;
    if (key_str.length() < 32) {
        key_str.append(32 - key_str.length(), '\0');
    } else if (key_str.length() > 32) {
        key_str = key_str.substr(0, 32);
    }

    AES_set_encrypt_key(reinterpret_cast<const unsigned char*>(key_str.c_str()), AES_KEY_LENGTH, &key);
    memcpy(original_ivec, ivec_str.c_str(), AES_BLOCK_SIZEE);
    resetIV();
}

std::string AESEncryptor::encrypt(const std::string& plaintext) {
    std::vector<unsigned char> outdata(plaintext.size());
    int num = 0;

    resetIV();

    AES_cfb128_encrypt(
        reinterpret_cast<const unsigned char*>(plaintext.c_str()),
        outdata.data(),
        plaintext.size(),
        &key,
        ivec,
        &num,
        AES_ENCRYPT
    );

    return std::string(reinterpret_cast<char*>(outdata.data()), outdata.size());
}

std::string AESEncryptor::decrypt(const std::string& encrypted_text) {
    std::vector<unsigned char> outdata(encrypted_text.size());
    int num = 0;

    resetIV();

    AES_cfb128_encrypt(
        reinterpret_cast<const unsigned char*>(encrypted_text.c_str()),
        outdata.data(),
        encrypted_text.size(),
        &key,
        ivec,
        &num,
        AES_DECRYPT
    );

    return std::string(reinterpret_cast<char*>(outdata.data()), outdata.size());
}
