#include <iostream>
#include <nlohmann/json.hpp>
#include <rpc/server.h>
#include <cryptopp/rsa.h>
#include <cryptopp/rng.h>
#include <cryptopp/osrng.h>

unsigned char AES_key[32];

std::string request_key(const std::string& req_json){
    nlohmann::json req = nlohmann::json::parse(req_json);
    std::string smod = req["pub_mod"];
    std::string sexp = req["pub_exp"];
    CryptoPP::Integer mod = CryptoPP::Integer(smod.c_str());
    CryptoPP::Integer exp = CryptoPP::Integer(sexp.c_str());
    CryptoPP::RSA::PublicKey pubkey;
    pubkey.Initialize(mod, exp);
    CryptoPP::RSAES_OAEP_SHA_Encryptor e(pubkey);

    std::stringstream ss;
    ss << AES_key;
    std::string key = ss.str();

    CryptoPP::AutoSeededRandomPool prng;
    std::string cipher;
    CryptoPP::StringSource ss1(key, true,new CryptoPP::PK_EncryptorFilter(prng, e,new CryptoPP::StringSink(cipher)));

    return cipher;
}

std::string make_request(const std::string& req_json){

}



int main(int argc, char** argv) {
    //TODO: cmd (imgui) args: port to run on
    rpc::server server(4000);

    //TODO: enryption
    //TODO: generate AES key at start, change every hour

    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(AES_key, sizeof(AES_key));

    server.bind("key", &request_key);
    server.bind("request", &make_request);


    server.run();
    return 0;
}
