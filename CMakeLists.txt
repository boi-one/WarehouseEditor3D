cmake_minimum_required(VERSION 3.19)

set(CMAKE_TOOLCHAIN_FILE "C:/local/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file" FORCE)

include(${CMAKE_TOOLCHAIN_FILE})

set(CMAKE_CXX_STANDARD 20)

set(VCPKG_DEFAULT_TRIPLET "x64-windows")

project(WarehouseEditor3D)

find_package(OpenGL REQUIRED)

find_package(SDL2 CONFIG REQUIRED)

find_package(ImGui REQUIRED)

find_package(GLEW REQUIRED)

find_package(nlohmann_json 3.2.0 REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h src/*.vert src/*.frag)

configure_file(src/shader.vert ${CMAKE_BINARY_DIR}/shader.vert COPYONLY)
configure_file(src/shader.frag ${CMAKE_BINARY_DIR}/shader.frag COPYONLY)

add_executable(${PROJECT_NAME} src/main.cpp ${SRC_FILES})


target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 GLEW::GLEW OpenGL::GL imgui::imgui nlohmann_json::nlohmann_json)