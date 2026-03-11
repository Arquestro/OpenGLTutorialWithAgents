#include <gl/Shader.hpp>

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

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

    bool IsProgramCurrentlyBound(GLuint handle)
    {
        GLint current_program = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
        return static_cast<GLuint>(current_program) == handle;
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
        UNIFORM_LOCATIONS.clear();
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
        UNIFORM_LOCATIONS.clear();
        return CheckProgramLink(HANDLE);
    }

    void ShaderProgram::Use() const
    {
        assert(HANDLE != 0);
        glUseProgram(HANDLE);
    }

    bool ShaderProgram::SetUniformBool(const char *name, bool value)
    {
        return SetUniformInt(name, value ? 1 : 0);
    }

    bool ShaderProgram::SetUniformFloat(const char *name, float value)
    {
        if (!EnsureProgramIsInUse())
        {
            return false;
        }
        GLint location = GetUniformLocation(name);
        if (location < 0)
        {
            return false;
        }
        glUniform1f(location, value);
        return true;
    }

    bool ShaderProgram::SetUniformInt(const char *name, int value)
    {
        if (!EnsureProgramIsInUse())
        {
            return false;
        }
        GLint location = GetUniformLocation(name);
        if (location < 0)
        {
            return false;
        }
        glUniform1i(location, value);
        return true;
    }

    bool ShaderProgram::SetUniformVec2(const char *name, const float *values)
    {
        assert(values != nullptr);
        if (!EnsureProgramIsInUse())
        {
            return false;
        }
        GLint location = GetUniformLocation(name);
        if (location < 0)
        {
            return false;
        }
        glUniform2fv(location, 1, values);
        return true;
    }

    bool ShaderProgram::SetUniformVec3(const char *name, const float *values)
    {
        assert(values != nullptr);
        if (!EnsureProgramIsInUse())
        {
            return false;
        }
        GLint location = GetUniformLocation(name);
        if (location < 0)
        {
            return false;
        }
        glUniform3fv(location, 1, values);
        return true;
    }

    bool ShaderProgram::SetUniformVec4(const char *name, const float *values)
    {
        assert(values != nullptr);
        if (!EnsureProgramIsInUse())
        {
            return false;
        }
        GLint location = GetUniformLocation(name);
        if (location < 0)
        {
            return false;
        }
        glUniform4fv(location, 1, values);
        return true;
    }

    bool ShaderProgram::SetUniformMat3(const char *name, const float *values, bool transpose)
    {
        assert(values != nullptr);
        if (!EnsureProgramIsInUse())
        {
            return false;
        }
        GLint location = GetUniformLocation(name);
        if (location < 0)
        {
            return false;
        }
        glUniformMatrix3fv(location, 1, transpose ? GL_TRUE : GL_FALSE, values);
        return true;
    }

    bool ShaderProgram::SetUniformMat4(const char *name, const float *values, bool transpose)
    {
        assert(values != nullptr);
        if (!EnsureProgramIsInUse())
        {
            return false;
        }
        GLint location = GetUniformLocation(name);
        if (location < 0)
        {
            return false;
        }
        glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, values);
        return true;
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
        UNIFORM_LOCATIONS = std::move(other.UNIFORM_LOCATIONS);
        other.HANDLE = 0;
        other.UNIFORM_LOCATIONS.clear();
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
        UNIFORM_LOCATIONS = std::move(other.UNIFORM_LOCATIONS);
        other.HANDLE = 0;
        other.UNIFORM_LOCATIONS.clear();
        return *this;
    }

    GLuint ShaderProgram::GetHandle() const
    {
        return HANDLE;
    }

    bool ShaderProgram::EnsureProgramIsInUse() const
    {
        assert(HANDLE != 0);
        if (HANDLE == 0)
        {
            return false;
        }
        if (!IsProgramCurrentlyBound(HANDLE))
        {
            std::cerr << "ShaderProgram::Use() must be called before setting uniforms.\n";
            return false;
        }
        return true;
    }

    GLint ShaderProgram::GetUniformLocation(const char *name)
    {
        assert(name != nullptr);
        auto iterator = UNIFORM_LOCATIONS.find(name);
        if (iterator != UNIFORM_LOCATIONS.end())
        {
            return iterator->second;
        }
        GLint location = glGetUniformLocation(HANDLE, name);
        UNIFORM_LOCATIONS[name] = location;
        if (location < 0)
        {
            std::cerr << "Uniform not found or optimized out: " << name << '\n';
        }
        return location;
    }
}
