-- add_rules("mode.debug")
add_rules("mode.release")
set_languages("cxx23")
-- set_runtimes("MD")

-- set_policy("build.sanitizer.address", true)
-- set_policy("build.sanitizer.thread", true)
-- set_policy("build.sanitizer.memory", true)
-- set_policy("build.sanitizer.leak", true)
-- set_policy("build.sanitizer.undefined", true)

add_requires("rpclib", "nlohmann_json", "openssl3", "cpp-httplib", "implot")
-- add_requires("qt6widgets", {system = true})
-- set_toolchain("mingw")
add_requires("imgui", {configs = {glfw = true, opengl3 = true}})

target("Encryptor")
    set_kind("static")
    add_files("lib/*.cpp")
    add_packages("openssl3", "rpclib", "nlohmann_json", "cpp-httplib")
target("Server")
    set_kind("binary")
    add_files("server/*.cpp")
    add_packages("rpclib", "nlohmann_json", "openssl3", "cpp-httplib", "imgui", "implot")
target("Client")
    set_kind("binary")
    add_files("client/*.cpp")
    add_packages("rpclib", "nlohmann_json", "openssl3", "cpp-httplib")
target("QtServer")
    add_rules("qt.widgetapp")
    set_kind("binary")
    add_headerfiles("QT/Server/*.h")
    add_files("Qt/Server/*.cpp")
    add_packages("rpclib", "nlohmann_json", "openssl3", "cpp-httplib")
    add_frameworks("QtWidgets")
target("QtClient")
    add_rules("qt.widgetapp")
    set_kind("binary")
    add_headerfiles("QT/Client/*.h")
    add_files("Qt/Client/*.cpp")
    add_packages("rpclib", "nlohmann_json", "openssl3", "cpp-httplib")
    add_frameworks("QtWidgets")
target("BotnetLocal")
    set_kind("binary")
    add_files("BotnetLocal/*.cpp")
    add_packages("rpclib", "nlohmann_json", "openssl3", "cpp-httplib")