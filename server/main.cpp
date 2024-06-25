#include <iostream>
#include <nlohmann/json.hpp>
#include <rpc/server.h>
#include <cryptopp/rsa.h>
#include <cryptopp/rng.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>


unsigned char AES_key[CryptoPP::AES::MAX_KEYLENGTH];
unsigned char iv[CryptoPP::AES::BLOCKSIZE];
std::string AES_key_str;

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
    ss << AES_key_str;
    ss << iv;
    std::string key = ss.str();

    CryptoPP::AutoSeededRandomPool prng;
    std::string cipher;
    CryptoPP::StringSource ss1(key, true,new CryptoPP::PK_EncryptorFilter(prng, e,new CryptoPP::StringSink(cipher)));
    return cipher;
}

std::string make_request(const std::string& req_enc){
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption d;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e;

    CryptoPP::AutoSeededRandomPool prng;
    d.SetKeyWithIV(AES_key, CryptoPP::AES::MAX_KEYLENGTH, iv);
    e.SetKeyWithIV(AES_key, CryptoPP::AES::MAX_KEYLENGTH, iv);

    std::string req_json;
    CryptoPP::StringSource s(req_enc, true,new CryptoPP::StreamTransformationFilter(d,new CryptoPP::StringSink(req_json)));
    std::cout << "req_json: " << req_json << std::endl;
    nlohmann::json req = nlohmann::json::parse(req_json);
    std::string message = req["message"];
    if (!message.contains("hello")){
        std::string reply;
        CryptoPP::StringSource ss1("fail", true,new CryptoPP::StreamTransformationFilter(e,new CryptoPP::StringSink(reply)));
        return reply;
    }
    else{
        std::string reply;
        CryptoPP::StringSource ss1("success " + message, true,new CryptoPP::StreamTransformationFilter(e,new CryptoPP::StringSink(reply)));
        return reply;
    }

}



int main(int argc, char** argv) {
    int port = 4000;
    if (argc == 2) {
        port = std::atoi(argv[1]);
    }
    else{
        if (!glfwInit())
            return 1;
        GLFWwindow* window = glfwCreateWindow(1280, 720, "Server Configuration", nullptr, nullptr);
        if (window == nullptr)
            return 1;
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
        while (!glfwWindowShouldClose(window)){
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::InputInt("port", &port);
            if (ImGui::Button("start")){
                break;
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    rpc::server server(port);

    //TODO: enryption
    //TODO: generate AES key at start, change every hour

    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(AES_key, sizeof(AES_key));
    std::stringstream ss;
    ss << AES_key;
    AES_key_str = ss.str();
    prng.GenerateBlock(iv, sizeof(iv));

    server.bind("key", &request_key);
    server.bind("request", &make_request);


    server.run();
    return 0;
}
