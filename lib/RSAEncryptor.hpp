#ifndef RSAENCRYPTOR_H
#define RSAENCRYPTOR_H

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

#endif // RSAENCRYPTOR_H
