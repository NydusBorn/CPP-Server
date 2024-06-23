#include <iostream>
#include <nlohmann/json.hpp>
#include <rpc/client.h>
#include <cryptopp/rsa.h>
#include <cryptopp/rng.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>

int main() {
    rpc::client client("127.0.0.1", 4000);

    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PrivateKey rsa_pk;
    rsa_pk.GenerateRandomWithKeySize(rng, 4096);
    CryptoPP::RSA::PublicKey rsa_pb(rsa_pk);
    std::stringstream smod, sexp;
    smod << rsa_pk.GetModulus();
    sexp << rsa_pk.GetPublicExponent();
    std::map<std::string, std::string> data = {
        {"pub_mod", smod.str()},
        {"pub_exp", sexp.str()}
    };
    nlohmann::json req = data;
    auto result = client.call("key", req.dump()).as<std::string>();

    CryptoPP::RSAES_OAEP_SHA_Decryptor rsad(rsa_pk);
    unsigned char AES_key[CryptoPP::AES::MAX_KEYLENGTH];
    unsigned char iv[CryptoPP::AES::BLOCKSIZE];
    std::string recovered;
    CryptoPP::StringSource ss2(result, true,new CryptoPP::PK_DecryptorFilter(rng, rsad, new CryptoPP::StringSink(recovered)));

    std::cout << "recovered: " << recovered << std::endl;
    for (int i = 0; i < sizeof(AES_key); i++) {
        AES_key[i] = recovered[i];
    }

    for (int i = 0; i < sizeof(iv); i++) {
        iv[i] = recovered[sizeof(AES_key) + i];
    }

    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption d;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e;

    d.SetKeyWithIV(AES_key, 32, iv);
    e.SetKeyWithIV(AES_key, 32, iv);

    std::map<std::string, std::string> data2 = {
        {"message", "hello"}
    };
    std::string message = nlohmann::json(data2).dump();
    std::string encrypted;
    CryptoPP::StringSource ss3(message, true,new CryptoPP::StreamTransformationFilter(e,new CryptoPP::StringSink(encrypted)));
    encrypted = client.call("request", encrypted).as<std::string>();

    recovered = "";
    CryptoPP::StringSource ss4(encrypted, true,new CryptoPP::StreamTransformationFilter(d,new CryptoPP::StringSink(recovered)));
    std::cout << "decrypted: " << recovered << std::endl;

    return 0;
}
