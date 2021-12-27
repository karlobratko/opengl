#include "Window.h"

#include <iostream>
#include <sstream>
#include <algorithm>

#include "gl_utils.h"
#include "Logger.h"

Window::Window() :
	Window{ 
		defaultWindowTitle, 
		defaultWindowWidth, 
		defaultWindowHeight 
	} {
}

Window::Window(std::string title, int width, int height) :
	Window{ title, width, height, defaultWindowFullscreen } {
}

Window::Window(std::string title, int width, int height, bool fullscreen) :
	Window{
		title,
		width, height,
		fullscreen,
		Window::DefaultFramebufferSizeCallback,
		Window::DefaultErrorCallback
	} {
}

Window::Window(std::string title, int width, int height, bool fullscreen, framebufferSizeCallback fbSizeCb) :
	Window{
		title,
		width, height,
		fullscreen,
		fbSizeCb,
		Window::DefaultErrorCallback
	} {
}

Window::Window(std::string title, int width, int height, bool fullscreen, framebufferSizeCallback fbSizeCb, errorCallback errorCb) :
	m_Title			{ title },
	m_Width			{ width },
	m_Height		{ height },
	m_Fullscreen{ fullscreen },
	m_FbSizeCb	{ fbSizeCb },
	m_ErrorCb		{ errorCb } {
}

Window::~Window() {
	if (m_Window) {
		glfwDestroyWindow(m_Window);
	}
	glfwTerminate();
}

void Window::SetHint(int hint, int value) {
	if (m_Window) {
		throw std::exception{ "[ERROR::GLFW] : window already initialized" };
	}

	if (m_WindowHints.find(hint) == std::end(m_WindowHints)) {
		m_WindowHints.emplace(hint, value);
	}
	else {
		m_WindowHints.at(hint) = value;
	}
}

void Window::SetFramebufferSizeCallback(framebufferSizeCallback fbSizeCb) {
	if (m_Window) {
		throw std::exception{ "[ERROR::GLFW] : window already initialized" };
	}
	
	m_FbSizeCb = fbSizeCb;
}

void Window::SetErrorCallback(errorCallback errorCb) {
	if (m_Window) {
		throw std::exception{ "[ERROR::GLFW] : window already initialized" };
	}

	m_ErrorCb = errorCb;
}

void Window::SetProcessInputCallback(processInputCallback procInputCb) {
	m_ProcessInputCb = procInputCb;
}

void Window::Initialize() {
	if (m_Window) {
		throw std::exception{ "[ERROR::GLFW] : window already initialized" };
	}

	Logger::Instance(std::cout).Log() << "[INFO::GLFW] : GLFW vesion: " << glfwGetVersionString();

	if (m_ErrorCb) {
		glfwSetErrorCallback(m_ErrorCb);
	}
	if (!glfwInit()) {
		throw std::exception{ "[ERROR::GLFW] : could not init GLFW" };
	}

	std::for_each(
		std::begin(m_WindowHints),
		std::end(m_WindowHints),
		[](const std::pair<int, int>& hint) -> void {
			glfwWindowHint(hint.first, hint.second);
		}
	);

	if (m_Fullscreen) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
		m_Width = videoMode->width;
		m_Height = videoMode->height;
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), monitor, nullptr);
	}
	else {
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	}

	if (!m_Window) {
		glfwTerminate();
		throw std::exception{ "[ERROR::GLFW] : could not create window" };
	}

	if (m_FbSizeCb) {
		glfwSetFramebufferSizeCallback(m_Window, m_FbSizeCb);
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(defaultSwapInterval);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		std::ostringstream outStream{};
		outStream
			<< "[ERROR::GLEW] : GLEW initialization failed\n"
			<< glewGetErrorString(GLEW_VERSION);
		throw std::exception{ outStream.str().c_str() };
	}
	Logger::Instance(std::cout).Log() << "[INFO::GLEW] : GLEW version: " << glewGetString(GLEW_VERSION);
}

int Window::Width() const {
	return m_Width;
}

int Window::Height() const {
	return m_Height;
}

void Window::SwapBuffers() {
	glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(m_Window);
}

void Window::CalculateFPS() {
	static double prevSeconds = glfwGetTime();
	static int frameCount = 0;

	double curSeconds = glfwGetTime();
	double elapsedSeconds = curSeconds - prevSeconds;

	if (elapsedSeconds > 0.25) {
		prevSeconds = curSeconds;
		double fps = (double)frameCount / elapsedSeconds;

		std::ostringstream outStream{};
		outStream << m_Title << " @ fps: " << fps;
		glfwSetWindowTitle(m_Window, outStream.str().c_str());

		frameCount = 0;
	}

	frameCount++;
}

void Window::ProcessInput() {
	if (m_ProcessInputCb) {
		m_ProcessInputCb(m_Window);
	}
}

void Window::DefaultErrorCallback(int error, const char* description) {
	std::ostringstream outStream{};
	outStream
		<< "[ERROR::GLFW] : code " << error << " msg: " << description;
	throw std::exception{ outStream.str().c_str() };
}

void Window::DefaultFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glCall(glViewport(0, 0, width, height));
}