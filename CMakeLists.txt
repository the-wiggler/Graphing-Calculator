cmake_minimum_required(VERSION 3.15)
project(calc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_ttf CONFIG REQUIRED)

include_directories(src)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/numOutputs.cpp
    src/graphWindow.cpp
    src/ui.cpp
)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
