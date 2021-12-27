#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <cmath>
#include <cstdlib>

#include <stb_image/stb_image.h>
#include <strim/strim.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl_utils.h"
#include "Window.h"
#include "Shader.h"
#include "Logger.h"
#include "Vertex.h"

// TYPES
using ubyte = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;

#define RGBA(x) ((x >> 24) & 0xff) / 255.0f, ((x >> 16) & 0xff) / 255.0f, ((x >> 8) & 0xff) / 255.0f, ((x >> 0) & 0xff) / 255.0f

// WINDOW CONSTANTS
constexpr const char* WINDOW_NAME = "LearnOpenGL";
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// RESOURCES
constexpr const char* VERT_SHDR_LOCATION = "res/shaders/shader.vert";
constexpr const char* FRAG_SHDR_LOCATION = "res/shaders/shader.frag";

constexpr const char* VERTICES_LOCATION = "res/vertices/shape.vertices";
constexpr const char* INDICES_LOCATION = "res/vertices/shape.indices";

constexpr const char* TEXTURE_FACE_LOCATION = "res/textures/awesomeface.png";
constexpr const char* TEXTURE_CONTAINER_LOCATION = "res/textures/container.jpg";
constexpr const char* TEXTURE_KITTEN_LOCATION = "res/textures/kitten.jpg";

// WINDOW FUNCTIONS
void window_error_callback(int error, const char* description);
void window_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_process_input(GLFWwindow* window);

// OPENGL PARAMS LOGGER
void log_params();

// RESOURCES LOADER FUNCTION
template <typename T>
void data_from_file(std::vector<T>& vec, const char* fileName) {
	std::ifstream file{ fileName };
	if (file.is_open()) {
		std::copy(std::istream_iterator<T>(file), std::istream_iterator<T>(), std::back_inserter(vec));
		file.close();
	}
	else {
		std::ostringstream outStream{};
		outStream << "[ERROR] : could not access file " << fileName;
		throw std::exception{ outStream.str().c_str() };
	}
}

int main() {
	// WINDOW SETUP
	Window window{ WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, false, window_framebuffer_size_callback, window_error_callback };
	try {
		window.SetHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		window.SetHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		window.SetHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		window.SetHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif //__APPLE__
		window.SetHint(GLFW_SAMPLES, 4);

		window.SetProcessInputCallback(window_process_input);

		window.Initialize();
	}
	catch (const std::exception& err) {
		Logger::Instance(std::cerr).Log() << err.what();
		return 1;
	}
	log_params();

	Shader shader{};
	try {
		shader.LoadFromFile(GL_VERTEX_SHADER, VERT_SHDR_LOCATION);
		shader.LoadFromFile(GL_FRAGMENT_SHADER, FRAG_SHDR_LOCATION);
		shader.Link();
	}
	catch (const std::exception& err) {
		Logger::Instance(std::cerr).Log() << err.what();
		return 1;
	}

	uint VAO, VBO, EBO;
	{
		float vertices[] = {
			-0.5f, -0.5f,  0.0f,  RGBA(0xff0000ff),  0.0f, 0.0f,
			-0.5f,  0.5f,  0.0f,  RGBA(0xffff00ff),  0.0f, 1.0f,
			 0.5f,  0.5f,  0.0f,  RGBA(0x00ff00ff),  1.0f, 1.0f,
			 0.5f, -0.5f,  0.0f,  RGBA(0x0000ffff),  1.0f, 0.0f,
		};

		ubyte indices[] = {
			0, 1, 2,
			2, 3, 0,
		};

		glCall(glGenVertexArrays(1, &VAO));
		glCall(glGenBuffers(1, &VBO));
		glCall(glGenBuffers(1, &EBO));

		glCall(glBindVertexArray(VAO));

		glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

		try {
			glCall(glVertexAttribPointer(shader.GetAttributeLocation("aPosition"), 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const char*>(0 * sizeof(float))));
			glCall(glVertexAttribPointer(shader.GetAttributeLocation("aColor"), 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const char*>(3 * sizeof(float))));
			glCall(glVertexAttribPointer(shader.GetAttributeLocation("aTexCoord"), 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const char*>(7 * sizeof(float))));

			glCall(glEnableVertexAttribArray(shader.GetAttributeLocation("aPosition")));
			glCall(glEnableVertexAttribArray(shader.GetAttributeLocation("aColor")));
			glCall(glEnableVertexAttribArray(shader.GetAttributeLocation("aTexCoord")));
		}
		catch (const std::exception& err) {
			Logger::Instance(std::cerr).Log() << err.what();
			return 1;
		}

		glCall(glBindVertexArray(0));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	uint texture0;
	{
		glCall(glGenTextures(1, &texture0));
		glCall(glBindTexture(GL_TEXTURE_2D, texture0));

		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(1);
		ubyte* data = stbi_load(TEXTURE_KITTEN_LOCATION, &width, &height, &nrChannels, 0);

		if (data) {
			glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
			glCall(glGenerateMipmap(GL_TEXTURE_2D));
			stbi_image_free(data);
		}
		else {
			Logger::Instance(std::cerr).Log() << "[ERROR] : could not load texture";
		}

		glCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	uint texture1;
	{
		glCall(glGenTextures(1, &texture1));
		glCall(glBindTexture(GL_TEXTURE_2D, texture1));

		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(1);
		ubyte* data = stbi_load(TEXTURE_CONTAINER_LOCATION, &width, &height, &nrChannels, 0);

		if (data) {
			glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
			glCall(glGenerateMipmap(GL_TEXTURE_2D));
			stbi_image_free(data);
		}
		else {
			Logger::Instance(std::cerr).Log() << "[ERROR] : could not load texture";
		}

		glCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	shader.Bind();
	shader.SetUniform1i(shader.GetUniformLocation("texture0"), GL_TEXTURE0 - GL_TEXTURE0);
	shader.SetUniform1i(shader.GetUniformLocation("texture1"), GL_TEXTURE1 - GL_TEXTURE0);
	shader.UnBind();

	glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)); //GL_LINE
	while (!window.ShouldClose()) {
		window.CalculateFPS();
		glfwPollEvents();
		window.ProcessInput();

		glCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); //state-setting
		glCall(glClear(GL_COLOR_BUFFER_BIT)); //state-using

		shader.Bind();

		glm::mat4 trans = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ 0.0f });
		trans = glm::rotate(trans, glm::radians(static_cast<float>(glfwGetTime())), glm::vec3{ 0.0f, 0.0f, 1.0f });
		trans = glm::scale(trans, glm::vec3{ 1.0f });

		shader.SetUniformMatrix4fv(shader.GetUniformLocation("transform"), GL_FALSE, glm::value_ptr(trans));

		float texOpacity = sin(glfwGetTime()) / 2.0f + 0.5f;
		shader.SetUniform1f(shader.GetUniformLocation("TexOpacity"), texOpacity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glCall(glBindVertexArray(VAO));
		glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr));
		glCall(glBindVertexArray(0));
		shader.UnBind();

		window.SwapBuffers();
	}

	glCall(glBindVertexArray(VAO));
	try {
		glCall(glDisableVertexAttribArray(shader.GetAttributeLocation("aPosition")));
		glCall(glDisableVertexAttribArray(shader.GetAttributeLocation("aColor")));
		glCall(glDisableVertexAttribArray(shader.GetAttributeLocation("aTexCoord")));
	}
	catch (const std::exception& err) {
		Logger::Instance(std::cerr).Log() << err.what();
		return 1;
	}
	glCall(glBindVertexArray(0));

	glCall(glDeleteVertexArrays(1, &VAO));
	glCall(glDeleteBuffers(1, &VBO));
	glCall(glDeleteBuffers(1, &EBO));

	glCall(glDeleteTextures(1, &texture0));
	glCall(glDeleteTextures(1, &texture1));

	return EXIT_SUCCESS;
}

void window_error_callback(int error, const char* description) {
	Logger::Instance(std::cerr).Log() << u8"[ERROR::GLFW] : code: " << error << " msg: " << description;
}

void window_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glCall(glViewport(0, 0, width, height));
}

void window_process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void log_params() {
	unsigned int params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO
	};

	const char* string_rep[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO"
	};

	for (int i = 0; i < 10; ++i) {
		int v = 0;
		glCall(glGetIntegerv(params[i], &v));
		Logger::Instance(std::cout).Log() << "[INFO::GL] : (" << string_rep[i] << ": " << v << ")";
	}

	int v[2] = { 0, 0 };
	glCall(glGetIntegerv(params[10], v));
	Logger::Instance(std::cout).Log() << "[INFO::GL] : (" << string_rep[10] << ": [" << v[0] << ", " << v[1] << "])";

	unsigned char flag = 0;
	glCall(glGetBooleanv(params[11], &flag));
	Logger::Instance(std::cout).Log() << "[INFO::GL] : (" << string_rep[11] << ": " << (flag > 0 ? 1 : 0) << ")";
}