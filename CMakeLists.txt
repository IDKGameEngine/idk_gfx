cmake_minimum_required(VERSION 3.31)

project(idkgfx CXX)

find_package(volk CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)

option(IDK_SHARED "Build as shared library" OFF)
if(IDK_SHARED)
    add_library(${PROJECT_NAME} SHARED)
    set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
else()
    add_library(${PROJECT_NAME} STATIC)
endif()

target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/h"
    "${CMAKE_CURRENT_SOURCE_DIR}/h_"
    "${STEAMWORKS_INCLUDE_DIRS}"
)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE volk::volk GPUOpen::VulkanMemoryAllocator
    PUBLIC  imgui SDL3 SDL3_image libidk
)

target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GfxService.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/RenderEngine.cpp"
)

add_custom_target(
    idk_gfx_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/${IDK_ASSETS_DIRNAME} ${IDK_OUTPUT_DIR}/${IDK_ASSETS_DIRNAME}
)
add_dependencies(${PROJECT_NAME} idk_gfx_data)

install(CODE "execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/slangc.sh)")
