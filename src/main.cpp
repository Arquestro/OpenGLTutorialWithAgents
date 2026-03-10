#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
namespace {
	constexpr int kWindowWidth = 800;
	constexpr int kWindowHeight = 600;
	constexpr const char *kWindowTitle = "GLFW Starter";
	void glfwErrorCallback(int error, const char *description) {
		std::cerr << "GLFW Error (" << error << "): " << description << '\n';
	}
} // namespace
int main() {
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW.\n";
		return EXIT_FAILURE;
	}
	// Request an OpenGL context (actual rendering code can be added later).
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
	GLFWwindow *window =
	    glfwCreateWindow(kWindowWidth, kWindowHeight, kWindowTitle, nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window.\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync.
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}
