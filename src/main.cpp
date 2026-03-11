#include <gl/GlApi.hpp>
#include <gl/GlfwContext.hpp>
#include <gl/RectangleMesh.hpp>
#include <gl/ScopedPolygonMode.hpp>
#include <gl/Shader.hpp>
#include <gl/Window.hpp>
#include <resources/ShaderSourceLibrary.hpp>

#include <cassert>
#include <iostream>
#include <string_view>

namespace
{
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr const char *WINDOW_TITLE = "GLFW Starter";
    constexpr float CLEAR_COLOR_R = 0.08f;
    constexpr float CLEAR_COLOR_G = 0.12f;
    constexpr float CLEAR_COLOR_B = 0.18f;
    constexpr float CLEAR_COLOR_A = 1.0f;
    constexpr float FILLED_RECTANGLE_VERTICES[] = {-0.95f, -0.65f, 1.0f, 0.0f, 0.0f, -0.15f, -0.65f, 0.0f, 1.0f, 0.0f,
                                                   -0.15f, 0.65f,  0.0f, 0.0f, 1.0f, -0.95f, -0.65f, 1.0f, 0.0f, 0.0f,
                                                   -0.15f, 0.65f,  0.0f, 0.0f, 1.0f, -0.95f, 0.65f,  1.0f, 1.0f, 0.0f};
    constexpr float WIREFRAME_RECTANGLE_VERTICES[] = {0.15f, -0.65f, 1.0f, 0.0f, 0.0f, 0.95f, -0.65f, 0.0f, 1.0f, 0.0f,
                                                      0.95f, 0.65f,  0.0f, 0.0f, 1.0f, 0.15f, -0.65f, 1.0f, 0.0f, 0.0f,
                                                      0.95f, 0.65f,  0.0f, 0.0f, 1.0f, 0.15f, 0.65f,  1.0f, 1.0f, 0.0f};
    void GlfwErrorCallback(int error, const char *description)
    {
        std::cerr << "GLFW Error (" << error << "): " << description << '\n';
    }

    bool ConfigureGraphics(gl::Window &window, gl::ShaderProgram &program, gl::RectangleMesh &filled_rectangle,
                           gl::RectangleMesh &wireframe_rectangle, std::string_view shader_directory_path)
    {
        window.MakeContextCurrent();
        window.SetDefaultFramebufferCallback();
        window.SetViewportFromFramebuffer();
        if (!filled_rectangle.Create(FILLED_RECTANGLE_VERTICES,
                                     sizeof(FILLED_RECTANGLE_VERTICES) / sizeof(FILLED_RECTANGLE_VERTICES[0])))
        {
            return false;
        }
        if (!wireframe_rectangle.Create(WIREFRAME_RECTANGLE_VERTICES,
                                        sizeof(WIREFRAME_RECTANGLE_VERTICES) / sizeof(WIREFRAME_RECTANGLE_VERTICES[0])))
        {
            return false;
        }
        gl::Shader vertex_shader;
        gl::Shader fragment_shader;
        resources::ShaderSourceLibrary shader_source_library(shader_directory_path);
        resources::ShaderSourcePair shader_source_pair;
        bool shader_files_loaded = shader_source_library.LoadPairWithFallback(
            "rectangle.vert", "rectangle.frag", "error.vert", "error.frag", shader_source_pair);
        if (!shader_files_loaded)
        {
            return false;
        }
        if (!vertex_shader.CreateAndCompile(GL_VERTEX_SHADER, shader_source_pair.VERTEX_SOURCE.c_str(),
                                            "Vertex shader"))
        {
            return false;
        }
        if (!fragment_shader.CreateAndCompile(GL_FRAGMENT_SHADER, shader_source_pair.FRAGMENT_SOURCE.c_str(),
                                              "Fragment shader"))
        {
            return false;
        }
        if (!program.Create())
        {
            return false;
        }
        if (!program.Link(vertex_shader, fragment_shader))
        {
            return false;
        }
        window.SetSwapInterval(1);
        return true;
    }

    void RenderFrame(gl::ShaderProgram &program, const gl::RectangleMesh &filled_rectangle,
                     const gl::RectangleMesh &wireframe_rectangle)
    {
        glClearColor(CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A);
        glClear(GL_COLOR_BUFFER_BIT);
        program.Use();
        bool time_uniform_set = program.SetUniformFloat("U_TIME_DATA", static_cast<float>(glfwGetTime()));
        assert(time_uniform_set);
        {
            gl::ScopedPolygonMode scoped_polygon_mode(GL_FILL);
            filled_rectangle.Draw();
        }
        {
            gl::ScopedPolygonMode scoped_polygon_mode(GL_LINE);
            wireframe_rectangle.Draw();
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: glfw_starter <shader_directory_path>\n";
        return 1;
    }
    glfwSetErrorCallback(GlfwErrorCallback);
    gl::GlfwContext glfw_context;
    if (!glfw_context.Initialize())
    {
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    gl::Window window;
    if (!window.Create(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE))
    {
        return 1;
    }
    assert(window.GetNativeHandle() != nullptr);
    gl::ShaderProgram shader_program;
    gl::RectangleMesh filled_rectangle;
    gl::RectangleMesh wireframe_rectangle;
    if (!ConfigureGraphics(window, shader_program, filled_rectangle, wireframe_rectangle, argv[1]))
    {
        return 1;
    }
    while (!window.ShouldClose())
    {
        RenderFrame(shader_program, filled_rectangle, wireframe_rectangle);
        window.SwapBuffers();
        window.PollEvents();
    }
    return 0;
}
