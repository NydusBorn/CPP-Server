#include <iostream>
#include <nlohmann/json.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "../lib/Connector.cpp"

//TODO: organise into library (another target), Imgui stays in demo, client and server just use the library (same one with differing options)

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

    Connector server;
    rpc::server rpcserver(port);

    rpcserver.bind("key", [&server](){
       return server.keyreq();
    });
    rpcserver.bind("request", [&server](const std::string &req_enc){
        return server.mainreq(req_enc);
    });

    rpcserver.async_run(4);
    std::cout << "Server started on port " << port << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    return 0;
}
