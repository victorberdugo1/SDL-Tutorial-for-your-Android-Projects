cmake_minimum_required(VERSION 3.15)

# Declare the project
project(simple-triangle-android-opengles20-sdl3-cpp)

# Set the name of the executable
set(EXECUTABLE_NAME ${PROJECT_NAME})

add_library(${EXECUTABLE_NAME} SHARED src/main.cpp)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC SDL3::SDL3 SDL3_image::SDL3_image android EGL GLESv2)

target_sources(${EXECUTABLE_NAME}
        PRIVATE
        src/main.cpp
)

find_package(SDL3)
find_package(SDL3_image)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL3::SDL3)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL3_image::SDL3_image)
target_compile_definitions(${EXECUTABLE_NAME} PUBLIC SDL_MAIN_USE_CALLBACKS)
