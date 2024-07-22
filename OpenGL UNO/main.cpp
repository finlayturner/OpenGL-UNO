#include "headers/shader.h"
#include "headers/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader ourShader, float* varMix);

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
	Shader ourShader("shaders/shader.vs", "shaders/shader.fs");

	// Set up vertex data & buffers, and configure vertex attributes
	// -------------------------------------------------------------
	float triangleOne[] = {
		// Positions        // Colors
		-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // Top left
		 0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // Top right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom left
		 0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f // Bottom right
	};
	unsigned int indicies[] = {
		0, 1, 2,
		1, 2, 3
	};
	float texCoords[] = {
		0.0f, 0.0f,   // Lower-left
		1.0f, 0.0f,   // Lower-right
		0.5f, 1.0f    // Top-center
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// First triangle VAO setup
	// ------------------------
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Load texture file and create texture object
	// -------------------------------------------
	unsigned int texture[2];
	glGenTextures(2, texture);
	// Texture 1
	// ---------
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Set texture wrapping options of currently bound texture object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Load and generate texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// Texture 2
	// ---------
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	// Set texture wrapping options of currently bound texture object
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Load and generate texture
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// Polygon mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Unbind VAO so other VAO calls dont accidentally modify the current VAO
	glBindVertexArray(0);

	float varMix = 0.5;
	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture2", 1);
	ourShader.setFloat("varMix", varMix);

	// Main render loop
	// ----------------
	while (!glfwWindowShouldClose(window))
	{
		// Handle input
		// ------------
		processInput(window, ourShader, &varMix);
			
		// Render
		// ------
		glClearColor(0.7f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw triangle
		// -----------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		// Activate shader
		ourShader.use();
		// Draw triangle
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
void processInput(GLFWwindow* window, Shader ourShader, float* varMix)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		if (*varMix < 1.0)
		{
			*varMix = *varMix + 0.005;
			ourShader.setFloat("varMix", *varMix);
		}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		if (*varMix > 0.0)
		{
			*varMix = *varMix - 0.005;
			ourShader.setFloat("varMix", *varMix);
		}
}