#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>

class Window
{
public:
	using errorCallback = void(*)(int, const char*);
	using framebufferSizeCallback = void(*)(GLFWwindow*, int, int);
	using processInputCallback = void(*)(GLFWwindow*);

	Window();
	Window(std::string title, int width, int height);
	Window(std::string title, int width, int height, bool fullscreen);
	Window(std::string title, int width, int height, bool fullscreen, framebufferSizeCallback fbSizeCb);
	Window(std::string title, int width, int height, bool fullscreen, framebufferSizeCallback fbSizeCb, errorCallback errorCb);
	~Window();

	void SetHint(int hint, int value);
	void SetFramebufferSizeCallback(framebufferSizeCallback fbSizeCb);
	void SetErrorCallback(errorCallback errorCb);
	void SetProcessInputCallback(processInputCallback procInputCb);

	void Initialize();

	int Width() const;
	int Height() const;

	void SwapBuffers();
	bool ShouldClose();
	void CalculateFPS();
	void ProcessInput();


private:
	static void DefaultErrorCallback(int, const char*);
	static void DefaultFramebufferSizeCallback(GLFWwindow*, int, int);

	static constexpr const char* defaultWindowTitle = "OpenGL";
	static constexpr int defaultWindowWidth = 800;
	static constexpr int defaultWindowHeight = 600;
	static constexpr bool defaultWindowFullscreen = false;
	static constexpr int defaultSwapInterval = 1;

	GLFWwindow* m_Window = nullptr;
	std::string m_Title;
	int m_Width;
	int m_Height;
	bool m_Fullscreen;
	framebufferSizeCallback m_FbSizeCb;
	errorCallback m_ErrorCb;
	processInputCallback m_ProcessInputCb = nullptr;
	std::unordered_map<int, int> m_WindowHints{};
};

