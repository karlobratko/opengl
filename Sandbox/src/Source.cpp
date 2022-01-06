#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Core.h"
#include "Core/Window.h"
#include "Core/Shader.h"
#include "Core/Camera.h"

#include "Logging/Logger.h"

#include "Utility/SystemInfo/SystemInfo.h"

#include "Vendor/stb_image/stb_image.h"

// RESOURCES
constexpr const char* TEX_PATH_CONTAINER   = "res/textures/container.jpg";
constexpr const char* TEX_PATH_FACE        = "res/textures/awesomeface.png";
constexpr const char* TEX_PATH_KITTEN      = "res/textures/kitten.jpg";
constexpr const char* SHDR_VERT_PATH_BASIC = "res/shaders/basic.vert";
constexpr const char* SHDR_FRAG_PATH_BASIC = "res/shaders/basic.frag";

// WINDOW
constexpr const char* WINDOW_NAME   = "Sandbox";
constexpr int         WINDOW_WIDTH  = 800;
constexpr int         WINDOW_HEIGHT = 600;

// TIMING
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// CAMERA
Camera camera{ glm::vec3{ 0.0f, 0.0f, 3.0f } };

// FUNCTION DECLARATIONS
void WindowErrorCallback(int error, const char* description);
void MoveCamera(Window& window);

int main() 
{
  Window window{ WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, false, WindowErrorCallback };
  try
  {
    window.SetHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.SetHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.SetHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window.SetHint(GLFW_SAMPLES, 4);

    window.Initialize();
  }
  catch (const std::exception& err)
  {
    Logger::Instance(std::cerr).Log() << err.what();
    return EXIT_FAILURE;
  }
  SystemInfo();

  Shader shader{};
  try
  {
    shader.LoadFromFile(GL_VERTEX_SHADER, SHDR_VERT_PATH_BASIC);
    shader.LoadFromFile(GL_FRAGMENT_SHADER, SHDR_FRAG_PATH_BASIC);
    shader.Link();
  }
  catch (const std::exception& err)
  {
    Logger::Instance(std::cerr).Log() << err.what();
    return EXIT_FAILURE;
  }

  GLuint VAO, VBO;
  {
    GLfloat vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
       0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
       0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
       0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };

    /*GLfloat vertices[] = {
      -0.5f, -0.5f,  0.0f, RGBA(0xff0000ff), 0.0f, 0.0f,
      -0.5f,  0.5f,  0.0f, RGBA(0xffff00ff), 0.0f, 1.0f,
       0.5f,  0.5f,  0.0f, RGBA(0x00ff00ff), 1.0f, 1.0f,
       0.5f, -0.5f,  0.0f, RGBA(0x0000ffff), 1.0f, 0.0f
    };*/

    CALL(glGenVertexArrays(1, &VAO));
    CALL(glGenBuffers(1, &VBO));

    CALL(glBindVertexArray(VAO));

    CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    try
    {
      CALL(glVertexAttribPointer(shader.GetAttributeLocation("position"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void*>(0 * sizeof(float))));
      CALL(glEnableVertexAttribArray(shader.GetAttributeLocation("position")));

      CALL(glVertexAttribPointer(shader.GetAttributeLocation("texCoordinates"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void*>(3 * sizeof(float))));
      CALL(glEnableVertexAttribArray(shader.GetAttributeLocation("texCoordinates")));
    }
    catch (const std::exception& err)
    {
      Logger::Instance(std::cerr).Log() << err.what();
      return EXIT_FAILURE;
    }

    CALL(glBindVertexArray(0));
    CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }

  GLuint texContainer;
  {
    CALL(glGenTextures(1, &texContainer));
    CALL(glBindTexture(GL_TEXTURE_2D, texContainer));

    CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLsizei width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    GLubyte* data = stbi_load(TEX_PATH_CONTAINER, &width, &height, &nrChannels, 0);

    if (data)
    {
      CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
      CALL(glGenerateMipmap(GL_TEXTURE_2D));
      stbi_image_free(data);
    }
    else
    {
      Logger::Instance(std::cerr).Log() << "[ERROR] Could not load texture, path: " << TEX_PATH_CONTAINER;
      return EXIT_FAILURE;
    }

    CALL(glBindTexture(GL_TEXTURE_2D, 0));
  }

  shader.Bind();
  shader.SetUniform1i(shader.GetUniformLocation("texture0"), GL_TEXTURE0 - GL_TEXTURE0);
  shader.UnBind();

  CALL(glEnable(GL_DEPTH_TEST));
  CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  while (!window.ShouldClose())
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    window.CalculateFPS();
    glfwPollEvents();
    
    MoveCamera(window);

    CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    shader.Bind();
    glm::mat4 model      = glm::mat4{ 1.0f };
    glm::mat4 view       = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetCameraZoom()), static_cast<float>(window.GetWidth()) / window.GetHeight(), 0.1f, 100.0f);

    shader.SetUniformMatrix4fv(shader.GetUniformLocation("model"),      GL_FALSE, glm::value_ptr(model));
    shader.SetUniformMatrix4fv(shader.GetUniformLocation("view"),       GL_FALSE, glm::value_ptr(view));
    shader.SetUniformMatrix4fv(shader.GetUniformLocation("projection"), GL_FALSE, glm::value_ptr(projection));

    CALL(glActiveTexture(GL_TEXTURE0));
    CALL(glBindTexture(GL_TEXTURE_2D, texContainer));

    CALL(glBindVertexArray(VAO));
    CALL(glDrawArrays(GL_TRIANGLES, 0, 36));
    CALL(glBindVertexArray(0));
    shader.UnBind();

    window.SwapBuffers();
  }

  CALL(glBindVertexArray(VAO));
  try
  {
    CALL(glDisableVertexAttribArray(shader.GetAttributeLocation("position")));
    CALL(glDisableVertexAttribArray(shader.GetAttributeLocation("texCoordinates")));
  }
  catch (const std::exception& err)
  {
    Logger::Instance(std::cerr).Log() << err.what();
    return EXIT_FAILURE;
  }
  CALL(glBindVertexArray(0));

  CALL(glDeleteVertexArrays(1, &VAO));
  CALL(glDeleteBuffers(1, &VBO));

  CALL(glDeleteTextures(1, &texContainer));

  shader.Dispose();

  return EXIT_SUCCESS;
}

void WindowErrorCallback(int error, const char* description)
{
  Logger::Instance(std::cerr).Log() << u8"[ERROR::GLFW] : code: " << error << " msg: " << description;
}

void MoveCamera(Window& window)
{
  if (window.GetKeyState(GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboardFPS(Camera::MovementDirection::FORWARD, deltaTime);
  if (window.GetKeyState(GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboardFPS(Camera::MovementDirection::BACKWARD, deltaTime);
  if (window.GetKeyState(GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboardFPS(Camera::MovementDirection::RIGHT, deltaTime);
  if (window.GetKeyState(GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboardFPS(Camera::MovementDirection::LEFT, deltaTime);

  camera.ProcessMouseMovement(window.GetMouseXOffset(), window.GetMouseYOffset());

  camera.ProcessMouseScroll(window.GetScrollYOffset());
}
