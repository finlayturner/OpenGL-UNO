#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	
	// Initialize & Configure GLFW
	// ---------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW window
	// ------------------
	GLFWwindow* window = glfwCreateWindow(800, 600, "Goat Coder", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD: load all opengl function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Build and compile shader program
	// --------------------------------
	Shader ourShader("shader.vs", "shader.fs");

	// Set up vertex data & buffers, and configure vertex attributes
	// -------------------------------------------------------------
	float triangleOne[] = {
		// Positions        // Colors
		0.0f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // Top
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // left
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// First triangle VAO setup
	// ------------------------
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Polygon mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Unbind VAO so other VAO calls dont accidentally modify the current VAO
	glBindVertexArray(0);

	// Main render loop
	// ----------------
	while (!glfwWindowShouldClose(window))
	{
		// Handle input
		// ------------
		processInput(window);

		// Render
		// ------
		glClearColor(0.7f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw triangle
		// -----------------
		// Activate shader
		ourShader.use();
		// Draw triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers, and poll IO events
		// --------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// Clear all allocated GLFW resources
	// ----------------------------------
	glfwTerminate();
	return 0;
}

// When window size is changed, this callback function executes
// ------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process key input, and react accordingly
// ----------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}