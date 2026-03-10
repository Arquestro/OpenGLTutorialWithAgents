#include "gl/GlfwContext.hpp"

#include <iostream>

#include "gl/GlApi.hpp"

namespace gl {
    bool GlfwContext::Initialize() {
        if (INITIALIZED) {
            return true;
        }
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW.\n";
            return false;
        }
        INITIALIZED = true;
        return true;
    }

    GlfwContext::~GlfwContext() {
        if (!INITIALIZED) {
            return;
        }
        glfwTerminate();
    }
}
