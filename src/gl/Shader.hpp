#pragma once

#include <gl/GlApi.hpp>

namespace gl
{
    class Shader
    {
    public:
        Shader() = default;
        ~Shader();
        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;
        Shader(Shader &&other) noexcept;
        Shader &operator=(Shader &&other) noexcept;
        bool CreateAndCompile(GLenum type, const char *source, const char *shader_name);
        GLuint GetHandle() const;

    private:
        GLuint HANDLE = 0;
    };

    class ShaderProgram
    {
    public:
        ShaderProgram() = default;
        ~ShaderProgram();
        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
        ShaderProgram(ShaderProgram &&other) noexcept;
        ShaderProgram &operator=(ShaderProgram &&other) noexcept;
        bool Create();
        bool Link(const Shader &vertex_shader, const Shader &fragment_shader);
        void Use() const;
        GLuint GetHandle() const;

    private:
        GLuint HANDLE = 0;
    };
}
