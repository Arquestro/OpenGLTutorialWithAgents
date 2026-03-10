#include <gl/Shader.hpp>

#include <cassert>
#include <iostream>
#include <string>

namespace
{
    bool CheckShaderCompilation(GLuint shader, const char *shader_name)
    {
        GLint is_compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_TRUE)
        {
            return true;
        }
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::string info_log(static_cast<size_t>(log_length), '\0');
        glGetShaderInfoLog(shader, log_length, nullptr, info_log.data());
        std::cerr << shader_name << " compilation failed:\n" << info_log << '\n';
        return false;
    }

    bool CheckProgramLink(GLuint program)
    {
        GLint is_linked = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_TRUE)
        {
            return true;
        }
        GLint log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        std::string info_log(static_cast<size_t>(log_length), '\0');
        glGetProgramInfoLog(program, log_length, nullptr, info_log.data());
        std::cerr << "Shader program link failed:\n" << info_log << '\n';
        return false;
    }
}

namespace gl
{
    bool Shader::CreateAndCompile(GLenum type, const char *source, const char *shader_name)
    {
        assert(source != nullptr);
        assert(shader_name != nullptr);
        assert(HANDLE == 0);
        HANDLE = glCreateShader(type);
        if (HANDLE == 0)
        {
            std::cerr << "Failed to create shader object.\n";
            return false;
        }
        glShaderSource(HANDLE, 1, &source, nullptr);
        glCompileShader(HANDLE);
        if (!CheckShaderCompilation(HANDLE, shader_name))
        {
            glDeleteShader(HANDLE);
            HANDLE = 0;
            return false;
        }
        return true;
    }

    Shader::~Shader()
    {
        if (HANDLE == 0)
        {
            return;
        }
        glDeleteShader(HANDLE);
    }

    Shader::Shader(Shader &&other) noexcept
    {
        HANDLE = other.HANDLE;
        other.HANDLE = 0;
    }

    Shader &Shader::operator=(Shader &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        if (HANDLE != 0)
        {
            glDeleteShader(HANDLE);
        }
        HANDLE = other.HANDLE;
        other.HANDLE = 0;
        return *this;
    }

    GLuint Shader::GetHandle() const
    {
        return HANDLE;
    }

    bool ShaderProgram::Create()
    {
        assert(HANDLE == 0);
        HANDLE = glCreateProgram();
        if (HANDLE == 0)
        {
            std::cerr << "Failed to create shader program object.\n";
            return false;
        }
        return true;
    }

    bool ShaderProgram::Link(const Shader &vertex_shader, const Shader &fragment_shader)
    {
        assert(HANDLE != 0);
        assert(vertex_shader.GetHandle() != 0);
        assert(fragment_shader.GetHandle() != 0);
        glAttachShader(HANDLE, vertex_shader.GetHandle());
        glAttachShader(HANDLE, fragment_shader.GetHandle());
        glLinkProgram(HANDLE);
        return CheckProgramLink(HANDLE);
    }

    void ShaderProgram::Use() const
    {
        assert(HANDLE != 0);
        glUseProgram(HANDLE);
    }

    ShaderProgram::~ShaderProgram()
    {
        if (HANDLE == 0)
        {
            return;
        }
        glDeleteProgram(HANDLE);
    }

    ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept
    {
        HANDLE = other.HANDLE;
        other.HANDLE = 0;
    }

    ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        if (HANDLE != 0)
        {
            glDeleteProgram(HANDLE);
        }
        HANDLE = other.HANDLE;
        other.HANDLE = 0;
        return *this;
    }

    GLuint ShaderProgram::GetHandle() const
    {
        return HANDLE;
    }
}
