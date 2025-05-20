cmake_minimum_required(VERSION 3.15)
project(calc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(SDL3_ttf REQUIRED)

include_directories(src)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/numOutputs.cpp
    src/graphWindow.cpp
    src/ui.cpp
)

target_link_libraries(calc
    PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
