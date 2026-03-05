cmake_minimum_required(VERSION 3.5)

project(idk_gfx VERSION 0.0.1)
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
    ${PROJECT_NAME} PUBLIC glad OpenGL::GL OpenGL::GLU SDL3::SDL3 libidk idk_core
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gl_renderer.cpp"
)
    

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)