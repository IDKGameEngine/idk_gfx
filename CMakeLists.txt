cmake_minimum_required(VERSION 3.31)

project(idkgfx CXX)

find_package(volk CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_compile_options(
    ${PROJECT_NAME} PRIVATE -Wno-missing-field-initializers
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE volk::volk # GPUOpen::VulkanMemoryAllocator
    PUBLIC  imgui SDL3 SDL3_image libidk
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
