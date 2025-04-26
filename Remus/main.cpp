#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int WIDTH = 1600;
const unsigned int HEIGHT = 800;

const float vertices[] = { // Two triangles needed to create a square.
	 0.5f,  0.5f, 0.0f, // Top right.
 	 0.5f, -0.5f, 0.0f, // Bottom right.
	-0.5f, -0.5f, 0.0f, // Bottom left.
	-0.5f,  0.5f, 0.0f, // Top left.
};

const unsigned int indices[] = {
	0, 1, 3, // First triangle.
	1, 2, 3  // Second Triangle.
};

const char *vertexShaderSource =
"#version 330 core \n"
"layout (location = 0) in vec3 aPos; \n"
"void main() { \n "
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n "
"}\0";

const char *fragmentShaderSource =
"#version 330 core \n"
"out vec4 FragColor; \n"
"void main() { \n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
"}\0";

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
		return -1;
	}
	glfwMakeContextCurrent(window);
	return 0;
}

// Initialize GLAD for abstraction.
bool initGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	return 0;
}

// Define a callback for resizing the window and OpenGL viewport.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// For now, only processes wireframe input and closing of program.
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		isWireframe = !isWireframe;
		if (isWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

// Compile vertex shader 
void compileVertexShader(unsigned int &vertexShader) {
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

// Compile fragment shader
void compileFragmentShader(unsigned int &fragmentShader) {
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

// Link vertex and fragment shaders to a shader program.
void linkShaders(unsigned int* shaderBuffer, unsigned int& shaderProgram) { // In this case, shaberBuffer = {vertexShader, fragmentShader} 
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaderBuffer[0]);
	glAttachShader(shaderProgram, shaderBuffer[1]);
	glLinkProgram(shaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::SHADER_LINK_FAILED\n" << infoLog << std::endl;
	}
}

// Initialize shaders by creating the vertex and fragment shaders then linking them to a predefined shader program.
void initShaders(unsigned int& shaderProgram) {
	unsigned int shaderBuffer[2]; // {vertexShader, fragmentShader} 
	compileVertexShader(shaderBuffer[0]);
	compileFragmentShader(shaderBuffer[1]);
	linkShaders(shaderBuffer, shaderProgram);

	// After linking to the shader program, the shader buffer can be retired.
	glDeleteShader(shaderBuffer[0]);
	glDeleteShader(shaderBuffer[1]);
}

// Entry for the program. 
int main() {
	// Setup window.
	GLFWwindow* window;
	if (initWindow(window) || initGLAD()) {
		return -1;
	}

	// Define viewport and sizing callback.
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Compile, link, and use the basic shaders for OpenGL to render.
	unsigned int shaderProgram;
	initShaders(shaderProgram);

	// Main window loop.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		unsigned int VBO, VAO, EBO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // Copy vertices array into the VBO buffer.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0); // Set vertex attribute pointers.
		glEnableVertexAttribArray(0);

		// Code here for more rendering~

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
