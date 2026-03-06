cmake_minimum_required(VERSION 3.5)

project(idk_gfx CXX)

# add_compile_options(-Werror -Wall -Wextra)
find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glad")


add_library(
    ${PROJECT_NAME} STATIC
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h"
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC glad OpenGL::GL OpenGL::GLU SDL3::SDL3 glm idk_core
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer_opengl.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/buffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/debug.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/framebuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/texture.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/underlyingtype.cpp"
)


add_custom_target(
    idk_gfx_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/assets ${IDK_OUTPUT_DIR}/assets
)
add_dependencies(${PROJECT_NAME} idk_gfx_assets)
