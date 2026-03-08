#include "idk/gfx/gfx.hpp"
#include "idk/gfx/renderer.hpp"
#include <iostream>

using namespace idk::gfx;


static void debugmsg_callback( GLenum source, GLenum type, GLuint id, GLenum severity,
                               GLsizei length, GLchar const* message, void const* user_param )
{
    (void)length;
    (void)user_param;

	auto const src_str = [source]() {
		switch (source)
		{
            default:                                return "UNKNOWN";
            case GL_DEBUG_SOURCE_API:               return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY:       return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION:       return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER:             return "OTHER";
		}
	}();

	auto const type_str = [type]() {
		switch (type)
		{
            default:                                return "UNKNOWN";
            case GL_DEBUG_TYPE_ERROR:               return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER:              return "MARKER";
            case GL_DEBUG_TYPE_OTHER:               return "OTHER";
		}
	}();

	auto const severity_str = [severity] {
		switch (severity)
        {
            default:                                return "UNKNOWN";
		    case GL_DEBUG_SEVERITY_NOTIFICATION:    return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW:             return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM:          return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH:            return "HIGH";
		}
	}();


	std::cout << src_str << ", "
              << type_str << ", "
              << severity_str << ", "
              << id << ": "
              << message << '\n';
}



void RenderEngine::debugOutputEnable()
{
    gl::Enable(GL_DEBUG_OUTPUT);
    gl::Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    gl::DebugMessageCallback(debugmsg_callback, nullptr);

    gl::DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_TRUE);

    VLOG_INFO("Enabled OpenGL debug messages");
}


void RenderEngine::debugOutputDisable()
{
    gl::DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
}





