# this is the build file for project 
# it is autogenerated by the xmake build system.
# do not edit by hand.

# project
cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0091 NEW)
project(BotnetLocal LANGUAGES CXX)

# target
add_executable(BotnetLocal "")
set_target_properties(BotnetLocal PROPERTIES OUTPUT_NAME "BotnetLocal")
set_target_properties(BotnetLocal PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/linux/x86_64/release")
target_include_directories(BotnetLocal SYSTEM PRIVATE
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/include
    /home/nydus/.xmake/packages/n/nlohmann_json/v3.11.3/ec1183311a224b34b5c138edda96fdde/include
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/include
    /home/nydus/.xmake/packages/c/cpp-httplib/0.15.3/10d0838bab72482e8b727994616a9af6/include
)
target_compile_options(BotnetLocal PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64>
    $<$<COMPILE_LANGUAGE:CXX>:-m64>
    $<$<COMPILE_LANGUAGE:C>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:-DNDEBUG>
)
set_target_properties(BotnetLocal PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(BotnetLocal PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(BotnetLocal PRIVATE $<$<CONFIG:Release>:-Ox -fp:fast>)
else()
    target_compile_options(BotnetLocal PRIVATE -O3)
endif()
if(MSVC)
else()
    target_compile_options(BotnetLocal PRIVATE -fvisibility=hidden)
endif()
if(MSVC)
    set_property(TARGET BotnetLocal PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
target_link_libraries(BotnetLocal PRIVATE
    rpc
    ssl
    crypto
    pthread
    dl
)
target_link_directories(BotnetLocal PRIVATE
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/lib
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/lib64
)
target_link_options(BotnetLocal PRIVATE
    -m64
)
target_sources(BotnetLocal PRIVATE
    BotnetLocal/main.cpp
)

# target
add_executable(Client "")
set_target_properties(Client PROPERTIES OUTPUT_NAME "Client")
set_target_properties(Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/linux/x86_64/release")
target_include_directories(Client SYSTEM PRIVATE
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/include
    /home/nydus/.xmake/packages/n/nlohmann_json/v3.11.3/ec1183311a224b34b5c138edda96fdde/include
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/include
    /home/nydus/.xmake/packages/c/cpp-httplib/0.15.3/10d0838bab72482e8b727994616a9af6/include
)
target_compile_options(Client PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64>
    $<$<COMPILE_LANGUAGE:CXX>:-m64>
    $<$<COMPILE_LANGUAGE:C>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:-DNDEBUG>
)
set_target_properties(Client PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(Client PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(Client PRIVATE $<$<CONFIG:Release>:-Ox -fp:fast>)
else()
    target_compile_options(Client PRIVATE -O3)
endif()
if(MSVC)
else()
    target_compile_options(Client PRIVATE -fvisibility=hidden)
endif()
if(MSVC)
    set_property(TARGET Client PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
target_link_libraries(Client PRIVATE
    rpc
    ssl
    crypto
    pthread
    dl
)
target_link_directories(Client PRIVATE
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/lib
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/lib64
)
target_link_options(Client PRIVATE
    -m64
)
target_sources(Client PRIVATE
    client/client.cpp
)

# target
add_library(Encryptor STATIC "")
set_target_properties(Encryptor PROPERTIES OUTPUT_NAME "Encryptor")
set_target_properties(Encryptor PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/linux/x86_64/release")
target_include_directories(Encryptor SYSTEM PRIVATE
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/include
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/include
    /home/nydus/.xmake/packages/n/nlohmann_json/v3.11.3/ec1183311a224b34b5c138edda96fdde/include
    /home/nydus/.xmake/packages/c/cpp-httplib/0.15.3/10d0838bab72482e8b727994616a9af6/include
)
target_compile_options(Encryptor PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64>
    $<$<COMPILE_LANGUAGE:CXX>:-m64>
    $<$<COMPILE_LANGUAGE:C>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:-DNDEBUG>
)
set_target_properties(Encryptor PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(Encryptor PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(Encryptor PRIVATE $<$<CONFIG:Release>:-Ox -fp:fast>)
else()
    target_compile_options(Encryptor PRIVATE -O3)
endif()
if(MSVC)
else()
    target_compile_options(Encryptor PRIVATE -fvisibility=hidden)
endif()
if(MSVC)
    set_property(TARGET Encryptor PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
target_link_libraries(Encryptor PRIVATE
    ssl
    crypto
    rpc
    pthread
    dl
)
target_link_directories(Encryptor PRIVATE
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/lib64
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/lib
)
target_link_options(Encryptor PRIVATE
    -m64
)
target_sources(Encryptor PRIVATE
    lib/AESEncryptor.cpp
    lib/RSAEncryptor.cpp
    lib/ConnectorBase.cpp
    lib/ConnectorSimple.cpp
    lib/ConnectorBotnet.cpp
    lib/BruteForcer.cpp
    lib/MapDecryptor.cpp
)

# target
add_executable(QtBotnet "")
set_target_properties(QtBotnet PROPERTIES OUTPUT_NAME "QtBotnet")
set_target_properties(QtBotnet PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/linux/x86_64/release")
target_include_directories(QtBotnet PRIVATE
    build/.gens/QtBotnet/linux/x86_64/release/rules/qt/ui
)
target_include_directories(QtBotnet SYSTEM PRIVATE
    /usr/include/qt6/QtGui
    /usr/include/qt6/QtGui/6.7.2
    /usr/include/qt6/QtGui/6.7.2/QtGui
    /usr/include/qt6/QtWidgets
    /usr/include/qt6/QtWidgets/6.7.2
    /usr/include/qt6/QtWidgets/6.7.2/QtWidgets
    /usr/include/qt6/QtCore
    /usr/include/qt6/QtCore/6.7.2
    /usr/include/qt6/QtCore/6.7.2/QtCore
    /usr/include/qt6
    /usr/lib64/qt6/mkspecs/linux-g++
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/include
    /home/nydus/.xmake/packages/n/nlohmann_json/v3.11.3/ec1183311a224b34b5c138edda96fdde/include
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/include
    /home/nydus/.xmake/packages/c/cpp-httplib/0.15.3/10d0838bab72482e8b727994616a9af6/include
)
target_compile_definitions(QtBotnet PRIVATE
    QT_NO_DEBUG
    QT_DEPRECATED_WARNINGS
    QT_GUI_LIB
    QT_WIDGETS_LIB
    QT_CORE_LIB
)
target_compile_options(QtBotnet PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64>
    $<$<COMPILE_LANGUAGE:CXX>:-m64>
    $<$<COMPILE_LANGUAGE:C>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:C>:-fPIC>
    $<$<COMPILE_LANGUAGE:CXX>:-fPIC>
)
set_target_properties(QtBotnet PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(QtBotnet PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(QtBotnet PRIVATE $<$<CONFIG:Release>:-Ox -fp:fast>)
else()
    target_compile_options(QtBotnet PRIVATE -O3)
endif()
if(MSVC)
else()
    target_compile_options(QtBotnet PRIVATE -fvisibility=hidden)
endif()
if(MSVC)
    set_property(TARGET QtBotnet PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()
target_link_libraries(QtBotnet PRIVATE
    rpc
    ssl
    crypto
    Qt6Gui
    Qt6Widgets
    Qt6Core
    pthread
    dl
)
target_link_directories(QtBotnet PRIVATE
    /usr/lib64
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/lib
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/lib64
)
target_link_options(QtBotnet PRIVATE
    -m64
)
target_sources(QtBotnet PRIVATE
    Qt/Botnet/main.cpp
)

# target
add_executable(QtClient "")
set_target_properties(QtClient PROPERTIES OUTPUT_NAME "QtClient")
set_target_properties(QtClient PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/linux/x86_64/release")
target_include_directories(QtClient PRIVATE
    build/.gens/QtClient/linux/x86_64/release/rules/qt/ui
)
target_include_directories(QtClient SYSTEM PRIVATE
    /usr/include/qt6/QtGui
    /usr/include/qt6/QtGui/6.7.2
    /usr/include/qt6/QtGui/6.7.2/QtGui
    /usr/include/qt6/QtWidgets
    /usr/include/qt6/QtWidgets/6.7.2
    /usr/include/qt6/QtWidgets/6.7.2/QtWidgets
    /usr/include/qt6/QtCore
    /usr/include/qt6/QtCore/6.7.2
    /usr/include/qt6/QtCore/6.7.2/QtCore
    /usr/include/qt6
    /usr/lib64/qt6/mkspecs/linux-g++
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/include
    /home/nydus/.xmake/packages/n/nlohmann_json/v3.11.3/ec1183311a224b34b5c138edda96fdde/include
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/include
    /home/nydus/.xmake/packages/c/cpp-httplib/0.15.3/10d0838bab72482e8b727994616a9af6/include
)
target_compile_definitions(QtClient PRIVATE
    QT_NO_DEBUG
    QT_DEPRECATED_WARNINGS
    QT_GUI_LIB
    QT_WIDGETS_LIB
    QT_CORE_LIB
)
target_compile_options(QtClient PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64>
    $<$<COMPILE_LANGUAGE:CXX>:-m64>
    $<$<COMPILE_LANGUAGE:C>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:C>:-fPIC>
    $<$<COMPILE_LANGUAGE:CXX>:-fPIC>
)
set_target_properties(QtClient PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(QtClient PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(QtClient PRIVATE $<$<CONFIG:Release>:-Ox -fp:fast>)
else()
    target_compile_options(QtClient PRIVATE -O3)
endif()
if(MSVC)
else()
    target_compile_options(QtClient PRIVATE -fvisibility=hidden)
endif()
if(MSVC)
    set_property(TARGET QtClient PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()
target_link_libraries(QtClient PRIVATE
    rpc
    ssl
    crypto
    Qt6Gui
    Qt6Widgets
    Qt6Core
    pthread
    dl
)
target_link_directories(QtClient PRIVATE
    /usr/lib64
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/lib
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/lib64
)
target_link_options(QtClient PRIVATE
    -m64
)
target_sources(QtClient PRIVATE
    Qt/Client/main.cpp
)

# target
add_executable(QtServer "")
set_target_properties(QtServer PROPERTIES OUTPUT_NAME "QtServer")
set_target_properties(QtServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/linux/x86_64/release")
target_include_directories(QtServer PRIVATE
    build/.gens/QtServer/linux/x86_64/release/rules/qt/ui
)
target_include_directories(QtServer SYSTEM PRIVATE
    /usr/include/qt6/QtGui
    /usr/include/qt6/QtGui/6.7.2
    /usr/include/qt6/QtGui/6.7.2/QtGui
    /usr/include/qt6/QtWidgets
    /usr/include/qt6/QtWidgets/6.7.2
    /usr/include/qt6/QtWidgets/6.7.2/QtWidgets
    /usr/include/qt6/QtCore
    /usr/include/qt6/QtCore/6.7.2
    /usr/include/qt6/QtCore/6.7.2/QtCore
    /usr/include/qt6
    /usr/lib64/qt6/mkspecs/linux-g++
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/include
    /home/nydus/.xmake/packages/n/nlohmann_json/v3.11.3/ec1183311a224b34b5c138edda96fdde/include
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/include
    /home/nydus/.xmake/packages/c/cpp-httplib/0.15.3/10d0838bab72482e8b727994616a9af6/include
)
target_compile_definitions(QtServer PRIVATE
    QT_NO_DEBUG
    QT_DEPRECATED_WARNINGS
    QT_GUI_LIB
    QT_WIDGETS_LIB
    QT_CORE_LIB
)
target_compile_options(QtServer PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64>
    $<$<COMPILE_LANGUAGE:CXX>:-m64>
    $<$<COMPILE_LANGUAGE:C>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:C>:-fPIC>
    $<$<COMPILE_LANGUAGE:CXX>:-fPIC>
)
set_target_properties(QtServer PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(QtServer PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(QtServer PRIVATE $<$<CONFIG:Release>:-Ox -fp:fast>)
else()
    target_compile_options(QtServer PRIVATE -O3)
endif()
if(MSVC)
else()
    target_compile_options(QtServer PRIVATE -fvisibility=hidden)
endif()
if(MSVC)
    set_property(TARGET QtServer PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()
target_link_libraries(QtServer PRIVATE
    rpc
    ssl
    crypto
    Qt6Gui
    Qt6Widgets
    Qt6Core
    pthread
    dl
)
target_link_directories(QtServer PRIVATE
    /usr/lib64
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/lib
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/lib64
)
target_link_options(QtServer PRIVATE
    -m64
)
target_sources(QtServer PRIVATE
    Qt/Server/main.cpp
)

# target
add_executable(Server "")
set_target_properties(Server PROPERTIES OUTPUT_NAME "Server")
set_target_properties(Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/linux/x86_64/release")
target_include_directories(Server SYSTEM PRIVATE
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/include
    /home/nydus/.xmake/packages/n/nlohmann_json/v3.11.3/ec1183311a224b34b5c138edda96fdde/include
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/include
    /home/nydus/.xmake/packages/c/cpp-httplib/0.15.3/10d0838bab72482e8b727994616a9af6/include
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/1e3bedcc00134776943939c8a54e344b/include
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/1e3bedcc00134776943939c8a54e344b/include/imgui
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/1e3bedcc00134776943939c8a54e344b/include/backends
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/1e3bedcc00134776943939c8a54e344b/include/misc/cpp
    /home/nydus/.xmake/packages/g/glfw/3.4/4c3c72af70fd4599a3038fc662af9c28/include
    /home/nydus/.xmake/packages/l/libxinerama/1.1.5/e539e5b3575342a89c7a931295fe40c5/include
    /home/nydus/.xmake/packages/l/libxcursor/1.2.1/0732a9883e0c44a2a5e1529e9d2dbe14/include
    /home/nydus/.xmake/packages/i/implot/v0.16/958552fb009040bf90338d7726b3a70d/include
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/2f20ce333d2141edbe6a67692a07023c/include
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/2f20ce333d2141edbe6a67692a07023c/include/imgui
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/2f20ce333d2141edbe6a67692a07023c/include/backends
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/2f20ce333d2141edbe6a67692a07023c/include/misc/cpp
)
target_compile_definitions(Server PRIVATE
    GLFW_INCLUDE_NONE
)
target_compile_options(Server PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64>
    $<$<COMPILE_LANGUAGE:CXX>:-m64>
    $<$<COMPILE_LANGUAGE:C>:-DNDEBUG>
    $<$<COMPILE_LANGUAGE:CXX>:-DNDEBUG>
)
set_target_properties(Server PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(Server PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(Server PRIVATE $<$<CONFIG:Release>:-Ox -fp:fast>)
else()
    target_compile_options(Server PRIVATE -O3)
endif()
if(MSVC)
else()
    target_compile_options(Server PRIVATE -fvisibility=hidden)
endif()
if(MSVC)
    set_property(TARGET Server PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
target_link_libraries(Server PRIVATE
    rpc
    ssl
    crypto
    imgui
    glfw3
    OpenGL
    Xrandr
    Xinerama
    Xcursor
    Xrender
    X11
    Xi
    Xfixes
    Xext
    xcb
    Xau
    implot
    pthread
    dl
)
target_link_directories(Server PRIVATE
    /home/nydus/.xmake/packages/r/rpclib/v2.3.0/cd9a29cac23a478da714214c55a59c0c/lib
    /home/nydus/.xmake/packages/o/openssl3/3.0.7/8aaa08615b8740ada8faf167c0cbf065/lib64
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/1e3bedcc00134776943939c8a54e344b/lib
    /home/nydus/.xmake/packages/g/glfw/3.4/4c3c72af70fd4599a3038fc662af9c28/lib
    /home/nydus/.xmake/packages/l/libxinerama/1.1.5/e539e5b3575342a89c7a931295fe40c5/lib
    /home/nydus/.xmake/packages/l/libxcursor/1.2.1/0732a9883e0c44a2a5e1529e9d2dbe14/lib
    /home/nydus/.xmake/packages/i/implot/v0.16/958552fb009040bf90338d7726b3a70d/lib
    /home/nydus/.xmake/packages/i/imgui/v1.90.8/2f20ce333d2141edbe6a67692a07023c/lib
)
target_link_options(Server PRIVATE
    -m64
)
target_sources(Server PRIVATE
    server/main.cpp
)

