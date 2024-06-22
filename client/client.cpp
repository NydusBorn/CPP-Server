#include <iostream>
#include <nlohmann/json.hpp>
#include <rpc/client.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/encoder.h>
#include <openssl/decoder.h>
#include <fstream>

int main() {
    rpc::client client("127.0.0.1", 4000);

    auto pkey = EVP_RSA_gen(4096);
    if (!pkey) {
        std::cerr << "Failed to generate RSA key" << std::endl;
        return 1;
    }

    FILE* tmp = tmpfile();

    BIO *bp = BIO_new_fp(tmp, BIO_NOCLOSE);
    PEM_write_bio_PUBKEY(bp, pkey);
    BIO_free(bp);
    std::string pubkey;

    fseek(tmp, 0, SEEK_SET);
    char buf[1024];
    size_t len = 0;
    while ((len = fread(buf, 1, sizeof(buf), tmp)) > 0) {
        pubkey.append(buf, len);
    }

    std::cout << pubkey << std::endl;

    std::map<std::string, std::string> data = {
        {"pubkey", pubkey}
    };
    nlohmann::json req = data;
    auto result = client.call("key", req.dump()).as<std::string>();
    std::cout << "result: " << result << std::endl;

    unsigned char key[32];
    auto lenk = std::make_unique<size_t>(sizeof(key));
    auto ctx = EVP_PKEY_CTX_new(pkey, NULL);
    unsigned char input[4096];
    for (int i = 0; i < result.length(); i++) {
        input[i] = result[i];
    }
    EVP_PKEY_decrypt(ctx, key, lenk.get(), input, result.length());

    std::cout << "key: " << key << std::endl;

    return 0;
}
