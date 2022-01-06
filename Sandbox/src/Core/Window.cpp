#include "Window.h"

#include <iostream>
#include <sstream>
#include <algorithm>

#include "Core/Core.h"
#include "Logging/Logger.h"

Window::Window() :
  Window(Window::defaultTitle ,
         Window::defaultWidth ,
         Window::defaultHeight)
{
}

Window::Window(std::string title, int width, int height) :
  Window(title, width, height, Window::defaultFullscreen)
{
}

Window::Window(std::string title, 
               int width        , 
               int height       , 
               bool fullscreen  ) :
  Window(title                ,
         width                ,
         height               ,
         fullscreen           ,
         Window::ErrorCallback)
{
}

Window::Window(std::string title    ,
               int width            ,
               int height           ,
               bool fullscreen      ,
               errorCallback errorCb) :
  m_Title     { title           },
  m_Width     { width           },
  m_Height    { height          },
  m_Fullscreen{ fullscreen      },
  m_ErrorCb   { errorCb         },
  m_MouseLastX{ m_Width / 2.0f  },
  m_MouseLastY{ m_Height / 2.0f } 
{
}

Window::~Window() 
{
  if (m_Window) {
    glfwDestroyWindow(m_Window);
  }
  glfwTerminate();
}

void Window::SetHint(int hint, int value) 
{
  if (m_Window) 
  {
    throw std::exception{ "[ERROR::GLFW] Window already initialized" };
  }

  if (m_WindowHints.find(hint) == std::end(m_WindowHints)) 
  {
    m_WindowHints.emplace(hint, value);
  }
  else 
  {
    m_WindowHints.at(hint) = value;
  }
}

void Window::SetFramebufferSizeCallback(framebufferSizeCallback callback) 
{
  if (m_Window) 
  {
    throw std::exception{ "[ERROR::GLFW] Window already initialized" };
  }

  m_FbSizeCb = callback;
}

void Window::SetErrorCallback(errorCallback callback)
{
  if (m_Window) 
  {
    throw std::exception{ "[ERROR::GLFW] Window already initialized" };
  }

  m_ErrorCb = callback;
}

void Window::SetKeyCallback(keyCallback callback)
{
  if (m_Window) 
  {
    throw std::exception{ "[ERROR::GLFW] Window already initialized" };
  }

  m_KeyCb = callback;
}

void Window::SetCursorPositionCallback(cursorPosCallback callback)
{
  if (m_Window) 
  {
    throw std::exception{ "[ERROR::GLFW] Window already initialized" };
  }

  m_CursorPositionCb = callback;
}

void Window::SetScrollCallback(scrollCallback callback)
{
  if (m_Window) 
  {
    throw std::exception{ "[ERROR::GLFW] Window already initialized" };
  }

  m_ScrollCb = callback;
}

void Window::Initialize() 
{
  if (m_Window) 
  {
    throw std::exception{ "[ERROR::GLFW] Window already initialized" };
  }

  Logger::Instance(std::cout).Log() << "[INFO::GLFW] GLFW vesion: " << glfwGetVersionString();

  glfwSetErrorCallback(m_ErrorCb);

  if (!glfwInit()) 
  {
    throw std::exception{ "[ERROR::GLFW] Could not init GLFW" };
  }

  std::for_each(
    std::begin(m_WindowHints),
    std::end(m_WindowHints),
    [](const std::pair<int, int>& hint) -> void {
      glfwWindowHint(hint.first, hint.second);
    }
  );

  if (m_Fullscreen)
  {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
    m_Width = videoMode->width;
    m_Height = videoMode->height;
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), monitor, nullptr);
  }
  else
  {
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
  }

  if (!m_Window)
  {
    glfwTerminate();
    throw std::exception{ "[ERROR::GLFW] Could not create window" };
  }

  glfwSetWindowUserPointer(m_Window, this);

  SetCallbacks();

  glfwMakeContextCurrent(m_Window);
  glfwSwapInterval(defaultSwapInterval);

  glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK)
  {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    std::ostringstream outStream{};
    outStream
      << "[ERROR::GLEW] GLEW initialization failed\n"
      << glewGetErrorString(GLEW_VERSION);
    throw std::exception{ outStream.str().c_str() };
  }
  Logger::Instance(std::cout).Log() << "[INFO::GLEW] GLEW version: " << glewGetString(GLEW_VERSION);

}

int Window::GetKeyState(int key) const
{
  return m_KeyboardKeys[key];
}

float Window::GetMouseXOffset()
{
  float xOffset = m_MouseXOffset;
  m_MouseXOffset = 0.0f;
  return xOffset;
}

float Window::GetMouseYOffset()
{
  float yOffset = m_MouseYOffset;
  m_MouseYOffset = 0.0f;
  return yOffset;
}

float Window::GetScrollXOffset()
{
  float xScroll = m_ScrollXOffset;
  m_ScrollXOffset = 0.0f;
  return xScroll;
}

float Window::GetScrollYOffset()
{
  float yScroll = m_ScrollYOffset;
  m_ScrollYOffset = 0.0f;
  return yScroll;
}

int Window::GetWidth() const
{
  return m_Width;
}

int Window::GetHeight() const
{
  return m_Height;
}

void Window::SwapBuffers()
{
  glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose()
{
  return glfwWindowShouldClose(m_Window);
}

void Window::CalculateFPS()
{
  static double prevSeconds = glfwGetTime();
  static int frameCount = 0;

  double curSeconds = glfwGetTime();
  double elapsedSeconds = curSeconds - prevSeconds;

  if (elapsedSeconds > 0.25)
  {
    prevSeconds = curSeconds;
    double fps = (double)frameCount / elapsedSeconds;

    std::ostringstream outStream{};
    outStream << m_Title << " @ fps: " << fps;
    glfwSetWindowTitle(m_Window, outStream.str().c_str());

    frameCount = 0;
  }

  frameCount++;
}

void Window::SetCallbacks()
{
  if (m_FbSizeCb) 
  {
    glfwSetFramebufferSizeCallback(m_Window, m_FbSizeCb);
  }
  else
  {
    glfwSetFramebufferSizeCallback(m_Window, Window::FramebufferSizeCallback);
  }

  if (m_KeyCb) 
  {
    glfwSetKeyCallback(m_Window, m_KeyCb);
  }
  else 
  {
    glfwSetKeyCallback(m_Window, Window::KeyCallback);
  }

  if (m_CursorPositionCb) 
  {
    glfwSetCursorPosCallback(m_Window, m_CursorPositionCb);
  }
  else 
  {
    glfwSetCursorPosCallback(m_Window, Window::MouseCallback);
  }

  if (m_ScrollCb) 
  {
    glfwSetScrollCallback(m_Window, m_ScrollCb);
  }
  else 
  {
    glfwSetScrollCallback(m_Window, Window::ScrollCallback);
  }

}

void Window::ErrorCallback(int error, const char* description)
{
  std::ostringstream outStream{};
  outStream
    << "[ERROR::GLFW] Code " << error << " Msg: " << description;
  throw std::exception{ outStream.str().c_str() };
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
  theWindow->m_Width = width;
  theWindow->m_Height = height;

  CALL(glViewport(0, 0, width, height));
}

void Window::KeyCallback(GLFWwindow* window, int key, int code, int action, int mode)
{
  Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
  
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (key >= 0 && key < 1024) 
  {
    if (action == GLFW_PRESS)
    {
      theWindow->m_KeyboardKeys[key] = GLFW_PRESS;
    }
    else if (action == GLFW_RELEASE)
    {
      theWindow->m_KeyboardKeys[key] = GLFW_RELEASE;
    }
  }
}

void Window::MouseCallback(GLFWwindow* window, double xOffset, double yOffset)
{
  Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

  if (theWindow->m_MouseInitialMovement) 
  {
    theWindow->m_MouseLastX = xOffset;
    theWindow->m_MouseLastY = yOffset;
    theWindow->m_MouseInitialMovement = false;
  }

  theWindow->m_MouseXOffset = xOffset - theWindow->m_MouseLastX;
  theWindow->m_MouseYOffset = theWindow->m_MouseLastY - yOffset;

  theWindow->m_MouseLastX = xOffset;
  theWindow->m_MouseLastY = yOffset;
}

void Window::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
  Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
  theWindow->m_ScrollXOffset = xOffset;
  theWindow->m_ScrollYOffset = yOffset;
}
