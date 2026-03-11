#pragma once

#include <gl/GlApi.hpp>

#include <string>
#include <unordered_map>

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
        bool SetUniformFloat(const char *name, float value);
        bool SetUniformBool(const char *name, bool value);
        bool SetUniformInt(const char *name, int value);
        bool SetUniformVec2(const char *name, const float *values);
        bool SetUniformVec3(const char *name, const float *values);
        bool SetUniformVec4(const char *name, const float *values);
        bool SetUniformMat3(const char *name, const float *values, bool transpose = false);
        bool SetUniformMat4(const char *name, const float *values, bool transpose = false);
        GLuint GetHandle() const;

    private:
        bool EnsureProgramIsInUse() const;
        GLint GetUniformLocation(const char *name);
        GLuint HANDLE = 0;
        std::unordered_map<std::string, GLint> UNIFORM_LOCATIONS;
    };
}
