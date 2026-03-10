#include <gl/Window.hpp>

#include <cassert>
#include <iostream>

namespace
{
    void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        (void)window;
        glViewport(0, 0, width, height);
    }
}

namespace gl
{
    bool Window::Create(int width, int height, std::string_view title)
    {
        assert(HANDLE == nullptr);
        HANDLE = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        if (HANDLE == nullptr)
        {
            std::cerr << "Failed to create GLFW window.\n";
            return false;
        }
        return true;
    }

    Window::~Window()
    {
        if (HANDLE == nullptr)
        {
            return;
        }
        glfwDestroyWindow(HANDLE);
    }

    Window::Window(Window &&other) noexcept
    {
        HANDLE = other.HANDLE;
        other.HANDLE = nullptr;
    }

    Window &Window::operator=(Window &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        if (HANDLE != nullptr)
        {
            glfwDestroyWindow(HANDLE);
        }
        HANDLE = other.HANDLE;
        other.HANDLE = nullptr;
        return *this;
    }

    bool Window::ShouldClose() const
    {
        assert(HANDLE != nullptr);
        return glfwWindowShouldClose(HANDLE) != 0;
    }

    void Window::SwapBuffers() const
    {
        assert(HANDLE != nullptr);
        glfwSwapBuffers(HANDLE);
    }

    void Window::PollEvents() const
    {
        glfwPollEvents();
    }

    void Window::MakeContextCurrent() const
    {
        assert(HANDLE != nullptr);
        glfwMakeContextCurrent(HANDLE);
    }

    void Window::SetSwapInterval(int interval) const
    {
        glfwSwapInterval(interval);
    }

    void Window::SetDefaultFramebufferCallback() const
    {
        assert(HANDLE != nullptr);
        glfwSetFramebufferSizeCallback(HANDLE, FramebufferSizeCallback);
    }

    void Window::SetViewportFromFramebuffer() const
    {
        assert(HANDLE != nullptr);
        int framebuffer_width = 0;
        int framebuffer_height = 0;
        glfwGetFramebufferSize(HANDLE, &framebuffer_width, &framebuffer_height);
        glViewport(0, 0, framebuffer_width, framebuffer_height);
    }

    GLFWwindow *Window::GetNativeHandle() const
    {
        return HANDLE;
    }
}
