#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <rpc/server.h>

unsigned char AES_key[32];

std::string request_key(const std::string& req_json){
    std::cout << "was called" << std::endl;
    std::cout << req_json << std::endl;
    nlohmann::json req = nlohmann::json::parse(req_json);
    std::string pubkey = req["pubkey"];
    std::cout << "pubkey: " << pubkey << std::endl;

    // Convert the public key string to RSA key
    BIO* bio = BIO_new_mem_buf(pubkey.c_str(), pubkey.length());
    RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio);

    // Encrypt the AES key using the RSA public key
    int encrypted_key_len = RSA_size(rsa);
    unsigned char encrypted_key[encrypted_key_len];
    unsigned char teststr[] = "AES_key";
    int result = RSA_public_encrypt(sizeof(teststr), teststr, encrypted_key, rsa, RSA_PKCS1_OAEP_PADDING);
    if (result == -1) {
        // Handle encryption error
        return "fail";
    }

    // Convert the encrypted key to a string
    std::string encrypted_key_str(reinterpret_cast<char*>(encrypted_key), encrypted_key_len);

    // Clean up
    RSA_free(rsa);

    // Return the encrypted key
    std::cout << "encrypted key: " << encrypted_key_str << std::endl;
    return encrypted_key_str;
}

std::string make_request(const std::string& req_json){

}



int main(int argc, char** argv) {
    //TODO: cmd (imgui) args: port to run on
    rpc::server server(4000);

    //TODO: enryption
    //TODO: generate AES key at start, change every hour
    RAND_bytes(AES_key, sizeof(AES_key));

    server.bind("key", &request_key);
    server.bind("request", &make_request);


    server.run();
    return 0;
}
