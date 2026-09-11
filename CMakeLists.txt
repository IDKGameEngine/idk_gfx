cmake_minimum_required(VERSION 4.2)

project(idk_gfx CXX)

find_package(OpenGL REQUIRED)

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
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h" "${CMAKE_CURRENT_SOURCE_DIR}/h_"
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC glad imgui OpenGL::GL OpenGL::GLU SDL3::SDL3-shared SDL3_image::SDL3_image libidk
)

if(IDK_GFX_MODEL STREQUAL "2D")
    target_compile_definitions(${PROJECT_NAME} PUBLIC IDK_GFX_MODEL_2D)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/gfx2d")
elseif(IDK_GFX_MODEL STREQUAL "3D")
    target_compile_definitions(${PROJECT_NAME} PUBLIC IDK_GFX_MODEL_3D)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/gfx3d")
else()
    message(FATAL_ERROR "Must set IDK_GFX_MODEL=\"2D|3D\"")
endif()

target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Controller.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GfxService.cpp"
)

add_custom_target(
    idk_gfx_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/${IDK_ASSETS_DIRNAME} ${IDK_OUTPUT_DIR}/${IDK_ASSETS_DIRNAME}
)
add_dependencies(${PROJECT_NAME} idk_gfx_data)

install(CODE "execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/slangc.sh)")
