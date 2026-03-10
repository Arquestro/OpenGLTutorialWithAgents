#include <gl/GlfwContext.hpp>

#include <gl/GlApi.hpp>

#include <iostream>

namespace gl
{
    bool GlfwContext::Initialize()
    {
        if (INITIALIZED)
        {
            return true;
        }
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW.\n";
            return false;
        }
        INITIALIZED = true;
        return true;
    }

    GlfwContext::~GlfwContext()
    {
        if (!INITIALIZED)
        {
            return;
        }
        glfwTerminate();
    }
}
