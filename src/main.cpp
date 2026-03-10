#include <GLFW/glfw3.h>
#include <iostream>

namespace {
	constexpr int WINDOW_WIDTH = 800;
	constexpr int WINDOW_HEIGHT = 600;
	constexpr const char *WINDOW_TITLE = "GLFW Starter";
	constexpr float CLEAR_COLOR_R = 0.08f;
	constexpr float CLEAR_COLOR_G = 0.12f;
	constexpr float CLEAR_COLOR_B = 0.18f;
	constexpr float CLEAR_COLOR_A = 1.0f;

	void glfwErrorCallback(int error, const char *description) {
		std::cerr << "GLFW Error (" << error << "): " << description << '\n';
	}

	void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
		(void)window;
		glViewport(0, 0, width, height);
	}
}

int main() {
	glfwSetErrorCallback(glfwErrorCallback);
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
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	int framebuffer_width = 0;
	int framebuffer_height = 0;
	glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
	glViewport(0, 0, framebuffer_width, framebuffer_height);
	glfwSwapInterval(1); // Enable vsync.
	while (!glfwWindowShouldClose(window)) {
		glClearColor(CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
