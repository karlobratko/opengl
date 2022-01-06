#pragma once

#include <unordered_map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window
{
public:
  using errorCallback = void(*)(int, const char*);
  using framebufferSizeCallback = void(*)(GLFWwindow*, int, int);
  using keyCallback = void(*)(GLFWwindow*, int, int, int, int);
  using cursorPosCallback = void(*)(GLFWwindow*, double, double);
  using scrollCallback = void(*)(GLFWwindow*, double, double);

  Window();
  Window(std::string title, int width, int height);
  Window(std::string title, int width, int height, bool fullscreen);
  Window(std::string title, int width, int height, bool fullscreen, errorCallback errorCb);
  ~Window();

  

  // SETTERS
  void SetHint(int hint, int value);

  void SetFramebufferSizeCallback(framebufferSizeCallback callback);
  void SetErrorCallback(errorCallback callback);
  void SetKeyCallback(keyCallback callback);
  void SetCursorPositionCallback(cursorPosCallback callback);
  void SetScrollCallback(scrollCallback callback);

  // INITIALIZE
  void Initialize();

  // GETTERS
  int GetWidth() const;
  int GetHeight() const;
  int GetKeyState(int key) const;

  float GetMouseXOffset();
  float GetMouseYOffset();
  float GetScrollXOffset();
  float GetScrollYOffset();

  // MANAGEMENT
  void SwapBuffers();
  bool ShouldClose();
  void CalculateFPS();

private:
  void SetCallbacks();

  // DEFAULT CALLBACKS
  static void ErrorCallback(int, const char*);
  static void FramebufferSizeCallback(GLFWwindow*, int, int);
  static void KeyCallback(GLFWwindow*, int, int, int, int);
  static void MouseCallback(GLFWwindow*, double, double);
  static void ScrollCallback(GLFWwindow*, double, double);

  static constexpr const char* defaultTitle = "OpenGL";
  static constexpr int defaultWidth         = 800;
  static constexpr int defaultHeight        = 600;
  static constexpr bool defaultFullscreen   = false;
  static constexpr int defaultSwapInterval  = 1;

  GLFWwindow* m_Window = nullptr;
  std::string m_Title;
  int m_Width;
  int m_Height;
  bool m_Fullscreen;

  errorCallback m_ErrorCb;
  framebufferSizeCallback m_FbSizeCb   = nullptr;
  keyCallback m_KeyCb                  = nullptr;
  cursorPosCallback m_CursorPositionCb = nullptr;
  scrollCallback m_ScrollCb            = nullptr;

  std::unordered_map<int, int> m_WindowHints{};

  // KEYBOARD INPUT
  int m_KeyboardKeys[1024]{};

  // MOUSE INPUT
  float m_MouseLastX;
  float m_MouseLastY;
  float m_MouseXOffset = 0.0f;
  float m_MouseYOffset = 0.0f;
  bool  m_MouseInitialMovement = true;

  // SCROLL INPUT
  float m_ScrollXOffset = 0.0f;
  float m_ScrollYOffset = 0.0f;

};