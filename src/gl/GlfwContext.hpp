#pragma once

namespace gl
{
    class GlfwContext
    {
    public:
        GlfwContext() = default;
        ~GlfwContext();
        GlfwContext(const GlfwContext &) = delete;
        GlfwContext &operator=(const GlfwContext &) = delete;
        GlfwContext(GlfwContext &&) = delete;
        GlfwContext &operator=(GlfwContext &&) = delete;
        bool Initialize();

    private:
        bool INITIALIZED = false;
    };
}
