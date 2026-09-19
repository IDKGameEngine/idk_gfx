target_sources(
    ${PROJECT_NAME} PRIVATE
    "RenderEngine.cpp"
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vk")
