#include <iostream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

namespace {
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
	constexpr const char *VERTEX_SHADER_SOURCE = R"(#version 330 core
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec3 a_color;
out vec3 v_color;
void main() {
	gl_Position = vec4(a_position, 0.0, 1.0);
	v_color = a_color;
}
)";
	constexpr const char *FRAGMENT_SHADER_SOURCE = R"(#version 330 core
in vec3 v_color;
out vec4 frag_color;
void main() {
	frag_color = vec4(v_color, 1.0);
}
)";

	void GlfwErrorCallback(int error, const char *description) {
		std::cerr << "GLFW Error (" << error << "): " << description << '\n';
	}

	void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
		(void)window;
		glViewport(0, 0, width, height);
	}

	bool CheckShaderCompilation(GLuint shader, const char *shader_name) {
		GLint is_compiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
		if (is_compiled == GL_TRUE) {
			return true;
		}
		GLint log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		std::string info_log(static_cast<size_t>(log_length), '\0');
		glGetShaderInfoLog(shader, log_length, nullptr, info_log.data());
		std::cerr << shader_name << " compilation failed:\n" << info_log << '\n';
		return false;
	}

	bool CheckProgramLink(GLuint program) {
		GLint is_linked = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
		if (is_linked == GL_TRUE) {
			return true;
		}
		GLint log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		std::string info_log(static_cast<size_t>(log_length), '\0');
		glGetProgramInfoLog(program, log_length, nullptr, info_log.data());
		std::cerr << "Shader program link failed:\n" << info_log << '\n';
		return false;
	}

	void CleanupGlResources(GLuint program, GLuint vertex_buffer_a, GLuint vertex_buffer_b, GLuint vertex_array_a,
	                        GLuint vertex_array_b) {
		if (program != 0) {
			glDeleteProgram(program);
		}
		if (vertex_buffer_a != 0) {
			glDeleteBuffers(1, &vertex_buffer_a);
		}
		if (vertex_buffer_b != 0) {
			glDeleteBuffers(1, &vertex_buffer_b);
		}
		if (vertex_array_a != 0) {
			glDeleteVertexArrays(1, &vertex_array_a);
		}
		if (vertex_array_b != 0) {
			glDeleteVertexArrays(1, &vertex_array_b);
		}
	}

	int CleanupAndExit(GLFWwindow *window, GLuint program, GLuint vertex_buffer_a, GLuint vertex_buffer_b,
	                   GLuint vertex_array_a, GLuint vertex_array_b, int exit_code) {
		CleanupGlResources(program, vertex_buffer_a, vertex_buffer_b, vertex_array_a, vertex_array_b);
		if (window != nullptr) {
			glfwDestroyWindow(window);
		}
		glfwTerminate();
		return exit_code;
	}
}

int main() {
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW.\n";
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window.\n";
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	int framebuffer_width = 0;
	int framebuffer_height = 0;
	glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
	glViewport(0, 0, framebuffer_width, framebuffer_height);
	GLuint filled_vertex_array = 0;
	GLuint filled_vertex_buffer = 0;
	GLuint wireframe_vertex_array = 0;
	GLuint wireframe_vertex_buffer = 0;
	GLuint vertex_shader = 0;
	GLuint fragment_shader = 0;
	GLuint shader_program = 0;
	glGenVertexArrays(1, &filled_vertex_array);
	glBindVertexArray(filled_vertex_array);
	glGenBuffers(1, &filled_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, filled_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FILLED_RECTANGLE_VERTICES), FILLED_RECTANGLE_VERTICES, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glGenVertexArrays(1, &wireframe_vertex_array);
	glBindVertexArray(wireframe_vertex_array);
	glGenBuffers(1, &wireframe_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, wireframe_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(WIREFRAME_RECTANGLE_VERTICES), WIREFRAME_RECTANGLE_VERTICES, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SOURCE, nullptr);
	glCompileShader(vertex_shader);
	if (!CheckShaderCompilation(vertex_shader, "Vertex shader")) {
		glDeleteShader(vertex_shader);
		return CleanupAndExit(window, 0, filled_vertex_buffer, wireframe_vertex_buffer, filled_vertex_array,
		                      wireframe_vertex_array, 1);
	}
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_SOURCE, nullptr);
	glCompileShader(fragment_shader);
	if (!CheckShaderCompilation(fragment_shader, "Fragment shader")) {
		glDeleteShader(fragment_shader);
		glDeleteShader(vertex_shader);
		return CleanupAndExit(window, 0, filled_vertex_buffer, wireframe_vertex_buffer, filled_vertex_array,
		                      wireframe_vertex_array, 1);
	}
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	if (!CheckProgramLink(shader_program)) {
		glDeleteShader(fragment_shader);
		glDeleteShader(vertex_shader);
		return CleanupAndExit(window, shader_program, filled_vertex_buffer, wireframe_vertex_buffer,
		                      filled_vertex_array, wireframe_vertex_array, 1);
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glfwSwapInterval(1); // Enable vsync.
	while (!glfwWindowShouldClose(window)) {
		glClearColor(CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(filled_vertex_array);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(wireframe_vertex_array);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return CleanupAndExit(window, shader_program, filled_vertex_buffer, wireframe_vertex_buffer, filled_vertex_array,
	                      wireframe_vertex_array, 0);
}
