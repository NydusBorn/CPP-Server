#include <iostream>
#include <nlohmann/json.hpp>
#include <rpc/client.h>
#include <cryptopp/rsa.h>
#include <cryptopp/rng.h>
#include <cryptopp/osrng.h>

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
    std::cout << "result: " << result << std::endl;

    CryptoPP::RSAES_OAEP_SHA_Decryptor d(rsa_pk);
    std::string recovered;
    CryptoPP::StringSource ss2(result, true,new CryptoPP::PK_DecryptorFilter(rng, d,new CryptoPP::StringSink(recovered)));

    std::cout << "recovered: " << recovered << std::endl;
    return 0;
}
