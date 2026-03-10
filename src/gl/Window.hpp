#pragma once

#include <gl/GlApi.hpp>

#include <string_view>

namespace gl
{
    class Window
    {
    public:
        Window() = default;
        ~Window();
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        Window(Window &&other) noexcept;
        Window &operator=(Window &&other) noexcept;
        bool Create(int width, int height, std::string_view title);
        bool ShouldClose() const;
        void SwapBuffers() const;
        void PollEvents() const;
        void MakeContextCurrent() const;
        void SetSwapInterval(int interval) const;
        void SetDefaultFramebufferCallback() const;
        void SetViewportFromFramebuffer() const;
        GLFWwindow *GetNativeHandle() const;

    private:
        GLFWwindow *HANDLE = nullptr;
    };
}
