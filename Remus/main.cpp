#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"

const unsigned int WIDTH = 1600;
const unsigned int HEIGHT = 800;

const float vertices[] = { // Two triangles needed to create a square.
	// Positions		// Colors
	 0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

const unsigned int indices[] = {
	0, 1, 2, // Triangle
};

bool isWireframe = false;

// Initialize window with GLFW.
bool initWindow(GLFWwindow*& window) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Remus Voxel Engine", NULL, NULL);
	if (window == nullptr) {
		std::cout << "Could not create engine window" << std::endl;
		glfwTerminate();
		return true;
	}
	glfwMakeContextCurrent(window);
	return false;
}

// Initialize GLAD for abstraction.
bool initGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return true;
	}
	return false;
}

// Define a callback for resizing the window and OpenGL viewport.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// A callback function for keyboard inputs.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	} else if (key == GLFW_KEY_W && action == GLFW_PRESS && action != GLFW_REPEAT) {
		isWireframe = !isWireframe;
		if (isWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

// Entry for the program. 
int main() {
	// Setup window.
	GLFWwindow* window;
	if (initWindow(window) || initGLAD()) {
		return -1;
	}

	// Define viewport and callback functions.
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << nrAttributes << std::endl;

	// Compile, link, and use the basic shaders for OpenGL to render.
	Shader ourShader("shader.vert", "shader.frag");

	// Set up buffers and configure vertex attributes.
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO); 
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Copy vertices array into the VBO buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0); // Set vertex position attribute pointers.
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float))); // Set vertex position attribute pointers.
	glEnableVertexAttribArray(1);

	// Unbind buffers and arrays.
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbinding VBO since VBO is regestired as the vertex attribute's bound vertex buffer object.
	glBindVertexArray(0);

	// Main window loop.
	while (!glfwWindowShouldClose(window)) {
		// Background and refresh.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap drawings / buffers and poll events.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(ourShader.ID);

	glfwTerminate();
	return 0;
}
