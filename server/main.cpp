#include <iostream>
#include <nlohmann/json.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "implot.h"
#include "../lib/ConnectorSimple.cpp"

//TODO: organise into library (another target), Imgui stays in demo, client and server just use the library (same one with differing options)

int main(int argc, char** argv) {
    int port = 4000;
    int threads = 4;
    if (!glfwInit())
        return 1;
    GLFWwindow* window = glfwCreateWindow(640, 720, "Server Configuration", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    enum class window_state{
        configuration,
        started
    };
    auto current_state = window_state::configuration;
    std::unique_ptr<ConnectorSimple> server;
    std::deque<double> times(1,0);
    std::deque<double> rps(1,0);
    auto start_time = std::chrono::steady_clock::now();
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y));
        switch (current_state) {
            case window_state::configuration:{
                glfwSetWindowTitle(window, "Server Configuration");
                ImGui::Begin("Server Configuration", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

                ImGui::InputInt("port", &port);
                if (port > 65535) {
                    port = 65535;
                }
                if (port < 0) {
                    port = 0;
                }
                ImGui::InputInt("threads", &threads);
                if (threads < 1) {
                    threads = 1;
                }
                if (threads > 2048) {
                    threads = 2048;
                }
                if (ImGui::Button("start")){
                    server = std::make_unique<ConnectorSimple>(port, threads);
                    current_state = window_state::started;
                    start_time = std::chrono::steady_clock::now();
                    break;
                }
                break;
            }
            case window_state::started:{
                glfwSetWindowTitle(window, "Server Statistics");
                ImGui::Begin("Server Statistics", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

                ImGui::Text("Public IP of this server: %s:%d", server->getPublicIP().c_str(), server->getPort());
                ImGui::Text("Threads: %d", threads);

                ImGui::Text("Unique client IDs seen this session: %d", server->getUniqueIDs());
                ImGui::Text("Unique client IPs seen this session: %d", server->getUniqueAddresses());

                if (times.size() > 500){
                    times.pop_front();
                    rps.pop_front();
                }

                double time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - start_time).count();
                times.push_back(time);
                rps.push_back(server->getRequestsPerSecond());
                ImGui::Text("Requests per second: %f", rps.back());


                if (ImPlot::BeginPlot("Requests per second")){
                    int size = times.size();
                    double time_c[500];
                    double rps_c[500];
                    for (int i = 0; i < std::min(size,500); i++){
                        time_c[i] = times[i];
                        rps_c[i] = rps[i];
                    }
                    ImPlot::SetNextAxesToFit();
                    ImPlot::PlotLine("RPS", time_c, rps_c, 500);
                    ImPlot::EndPlot();
                }
                break;
            }
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
