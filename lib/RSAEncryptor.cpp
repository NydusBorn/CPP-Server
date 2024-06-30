#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string>
#include <vector>
#include <stdexcept>

class RSAEncryptor {
private:
    RSA* rsa_keypair;
    int key_length;

    void generateKeyPair(int key_length);

public:
    RSAEncryptor(int key_length = 2048);
    RSAEncryptor(const std::string& key_pem, bool is_public);
    ~RSAEncryptor();

    std::string getPublicKey();
    std::string getPrivateKey();
    std::string encrypt(const std::string& plaintext);
    std::string decrypt(const std::string& encrypted_text) const;
    void loadKeyFromPEM(const std::string& key_pem, bool is_public);
};
void RSAEncryptor::generateKeyPair(int key_length) {
    this->key_length = key_length;
    rsa_keypair = RSA_new();
    BIGNUM* e = BN_new();
    BN_set_word(e, RSA_F4);

    if (!RSA_generate_key_ex(rsa_keypair, key_length, e, NULL)) {
        throw std::runtime_error("Ошибка генерации ключевой пары RSA");
    }

    BN_free(e);
}

RSAEncryptor::RSAEncryptor(int key_length) : rsa_keypair(nullptr) {
    generateKeyPair(key_length);
}

RSAEncryptor::RSAEncryptor(const std::string& key_pem, bool is_public) : rsa_keypair(nullptr) {
    loadKeyFromPEM(key_pem, is_public);
}

RSAEncryptor::~RSAEncryptor() {
    if (rsa_keypair != nullptr) {
        RSA_free(rsa_keypair);
    }
}

std::string RSAEncryptor::getPublicKey() {
    BIO* bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(bio, rsa_keypair);

    char* pem_key = NULL;
    long pem_key_len = BIO_get_mem_data(bio, &pem_key);

    std::string public_key(pem_key, pem_key_len);
    BIO_free(bio);

    return public_key;
}

std::string RSAEncryptor::getPrivateKey() {
    BIO* bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(bio, rsa_keypair, NULL, NULL, 0, NULL, NULL);

    char* pem_key = NULL;
    long pem_key_len = BIO_get_mem_data(bio, &pem_key);

    std::string private_key(pem_key, pem_key_len);
    BIO_free(bio);

    return private_key;
}

std::string RSAEncryptor::encrypt(const std::string& plaintext) {
    std::vector<unsigned char> encrypted_data(RSA_size(rsa_keypair));
    int encrypted_length = RSA_public_encrypt(
        plaintext.length(),
        reinterpret_cast<const unsigned char*>(plaintext.c_str()),
        encrypted_data.data(),
        rsa_keypair,
        RSA_PKCS1_OAEP_PADDING
    );

    if (encrypted_length == -1) {
        throw std::runtime_error("Ошибка шифрования данных: " + std::string(ERR_error_string(ERR_get_error(), NULL)));
    }

    return std::string(reinterpret_cast<char*>(encrypted_data.data()), encrypted_length);
}

std::string RSAEncryptor::decrypt(const std::string& encrypted_text) const {
    std::vector<unsigned char> decrypted_data(RSA_size(rsa_keypair));
    int decrypted_length = RSA_private_decrypt(
        encrypted_text.length(),
        reinterpret_cast<const unsigned char*>(encrypted_text.c_str()),
        decrypted_data.data(),
        rsa_keypair,
        RSA_PKCS1_OAEP_PADDING
    );

    if (decrypted_length == -1) {
        throw std::runtime_error("Ошибка расшифровки данных: " + std::string(ERR_error_string(ERR_get_error(), NULL)));
    }

    return std::string(reinterpret_cast<char*>(decrypted_data.data()), decrypted_length);
}

void RSAEncryptor::loadKeyFromPEM(const std::string& key_pem, bool is_public) {
    BIO* bio = BIO_new_mem_buf(key_pem.c_str(), -1);
    if (bio == nullptr) {
        throw std::runtime_error("Ошибка создания BIO объекта");
    }

    if (is_public) {
        rsa_keypair = PEM_read_bio_RSAPublicKey(bio, &rsa_keypair, NULL, NULL);
    } else {
        rsa_keypair = PEM_read_bio_RSAPrivateKey(bio, &rsa_keypair, NULL, NULL);
    }

    BIO_free(bio);

    if (rsa_keypair == nullptr) {
        throw std::runtime_error("Ошибка загрузки ключа из PEM");
    }
}
