cmake_minimum_required(VERSION 4.2)

project(idk_gfx CXX)

find_package(OpenGL REQUIRED)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glad")


add_library(
    ${PROJECT_NAME} STATIC
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h" "${CMAKE_CURRENT_SOURCE_DIR}/h_"
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC glad OpenGL::GL OpenGL::GLU SDL3::SDL3-shared SDL3_image::SDL3_image glm::glm-header-only idk_core
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx_api.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx_service.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx/buffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx/debug.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx/framebuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx/model.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx/renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx/shader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx/texture.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gfx/window.cpp"
)


add_custom_target(
    idk_gfx_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/asset ${IDK_OUTPUT_DIR}/asset
)
add_dependencies(${PROJECT_NAME} idk_gfx_assets)

install(CODE "execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/slangc.sh)")
