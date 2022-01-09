#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Core.h"
#include "Core/Window.h"
#include "Core/Shader.h"
#include "Core/Texture.h"
#include "Core/Camera.h"

#include "Logging/Logger.h"

#include "Utility/SystemInfo/SystemInfo.h"

#include "Vendor/stb_image/stb_image.h"

// RESOURCES
constexpr const char* TEX_PATH_CONTAINER   = "res/textures/container.jpg";
constexpr const char* TEX_PATH_FACE        = "res/textures/awesomeface.png";
constexpr const char* TEX_PATH_KITTEN      = "res/textures/kitten.jpg";

constexpr const char* SHDR_VERT_PATH_OBJ = "res/shaders/obj.vert";
constexpr const char* SHDR_VERT_PATH_LIGHT_SOURCE = "res/shaders/light_source.vert";
constexpr const char* SHDR_FRAG_PATH_OBJ = "res/shaders/obj.frag";
constexpr const char* SHDR_FRAG_PATH_LIGHT_SOURCE = "res/shaders/light_source.frag";

// WINDOW
constexpr const char* WINDOW_NAME   = "Sandbox";
constexpr int         WINDOW_WIDTH  = 800;
constexpr int         WINDOW_HEIGHT = 600;

// TIMING
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// CAMERA
Camera camera{ glm::vec3{ 0.0f, 0.0f, 3.0f } };

// LIGHT SOURCE
glm::vec3 lightPosition{ 1.2f, 1.0f, 2.0f };
glm::vec3 lightColor   { 1.0f, 1.0f, 1.0f };

// OBJECT 
glm::vec3 objectColor  { 1.0f, 0.5f, 0.31f };

// FUNCTION DECLARATIONS
void WindowErrorCallback(int error, const char* description);
void UpdateCamera(Window& window);
void UpdateLight(Window& window);

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

  Shader objShdr{};
  try
  {
    objShdr.LoadFromFile(GL_VERTEX_SHADER, SHDR_VERT_PATH_OBJ);
    objShdr.LoadFromFile(GL_FRAGMENT_SHADER, SHDR_FRAG_PATH_OBJ);
    objShdr.Link();
  }
  catch (const std::exception& err)
  {
    Logger::Instance(std::cerr).Log() << err.what();
    return EXIT_FAILURE;
  }

  Shader lightSrcShdr{};
  try
  {
    lightSrcShdr.LoadFromFile(GL_VERTEX_SHADER, SHDR_VERT_PATH_LIGHT_SOURCE);
    lightSrcShdr.LoadFromFile(GL_FRAGMENT_SHADER, SHDR_FRAG_PATH_LIGHT_SOURCE);
    lightSrcShdr.Link();
  }
  catch (const std::exception& err)
  {
    Logger::Instance(std::cerr).Log() << err.what();
    return EXIT_FAILURE;
  }

  GLuint VBO;
  {
    GLfloat vertices[] = {
      // positions           // normals            // tex
      -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
    };

    CALL(glGenBuffers(1, &VBO));
    CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }

  GLuint objectVAO;
  {
    CALL(glGenVertexArrays(1, &objectVAO));
    CALL(glBindVertexArray(objectVAO));
    CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));

    try
    {
      CALL(glVertexAttribPointer(objShdr.GetAttributeLocation("position"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void*>(0 * sizeof(float))));
      CALL(glEnableVertexAttribArray(objShdr.GetAttributeLocation("position")));

      CALL(glVertexAttribPointer(objShdr.GetAttributeLocation("normal"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void*>(3 * sizeof(float))));
      CALL(glEnableVertexAttribArray(objShdr.GetAttributeLocation("normal")));
    }
    catch (const std::exception& err)
    {
      Logger::Instance(std::cerr).Log() << err.what();
      return EXIT_FAILURE;
    }

    CALL(glBindVertexArray(0));
    CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }

  GLuint lightVAO;
  {
    CALL(glGenVertexArrays(1, &lightVAO));
    CALL(glBindVertexArray(lightVAO));
    CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));

    try
    {
      CALL(glVertexAttribPointer(lightSrcShdr.GetAttributeLocation("position"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void*>(0 * sizeof(float))));
      CALL(glEnableVertexAttribArray(lightSrcShdr.GetAttributeLocation("position")));
    }
    catch (const std::exception& err)
    {
      Logger::Instance(std::cerr).Log() << err.what();
      return EXIT_FAILURE;
    }

    CALL(glBindVertexArray(0));
    CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }

  CALL(glEnable(GL_DEPTH_TEST));
  CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  while (!window.ShouldClose())
  {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    window.CalculateFPS();
    glfwPollEvents();
    
    UpdateCamera(window);

    UpdateLight(window);

    CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    glm::mat4 view       = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(window.GetWidth()) / window.GetHeight(), 0.1f, 100.0f);

    {
      objShdr.Bind();
      glm::mat4 model = glm::mat4{ 1.0f };

      objShdr.SetUniformMatrix4fv(objShdr.GetUniformLocation("model"),      glm::value_ptr(model));
      objShdr.SetUniformMatrix4fv(objShdr.GetUniformLocation("view"),       glm::value_ptr(view));
      objShdr.SetUniformMatrix4fv(objShdr.GetUniformLocation("projection"), glm::value_ptr(projection));

      objShdr.SetUniform3fv(objShdr.GetUniformLocation("objectColor"),   glm::value_ptr(objectColor));
      objShdr.SetUniform3fv(objShdr.GetUniformLocation("lightColor"),    glm::value_ptr(lightColor));
      objShdr.SetUniform3fv(objShdr.GetUniformLocation("lightPosition"), glm::value_ptr(lightPosition));
      objShdr.SetUniform3fv(objShdr.GetUniformLocation("cameraPosition"), glm::value_ptr(camera.GetPosition()));

      CALL(glBindVertexArray(objectVAO));
      CALL(glDrawArrays(GL_TRIANGLES, 0, 36));
      CALL(glBindVertexArray(0));
      objShdr.UnBind();
    }

    {
      lightSrcShdr.Bind();
      glm::mat4 model = glm::translate(glm::mat4{ 1.0f }, lightPosition);
                model = glm::scale(model, glm::vec3{ 0.2f });

      lightSrcShdr.SetUniformMatrix4fv(lightSrcShdr.GetUniformLocation("model"),      glm::value_ptr(model));
      lightSrcShdr.SetUniformMatrix4fv(lightSrcShdr.GetUniformLocation("view"),       glm::value_ptr(view));
      lightSrcShdr.SetUniformMatrix4fv(lightSrcShdr.GetUniformLocation("projection"), glm::value_ptr(projection));

      CALL(glBindVertexArray(lightVAO));
      CALL(glDrawArrays(GL_TRIANGLES, 0, 36));
      CALL(glBindVertexArray(0));
      lightSrcShdr.UnBind();
    }

    window.SwapBuffers();
  }

  CALL(glBindVertexArray(objectVAO));
  try
  {
    CALL(glDisableVertexAttribArray(objShdr.GetAttributeLocation("position")));
    CALL(glDisableVertexAttribArray(objShdr.GetAttributeLocation("normal")));
  }
  catch (const std::exception& err)
  {
    Logger::Instance(std::cerr).Log() << err.what();
    return EXIT_FAILURE;
  }
  CALL(glBindVertexArray(0));

  CALL(glBindVertexArray(lightVAO));
  try
  {
    CALL(glDisableVertexAttribArray(lightSrcShdr.GetAttributeLocation("position")));
  }
  catch (const std::exception& err)
  {
    Logger::Instance(std::cerr).Log() << err.what();
    return EXIT_FAILURE;
  }
  CALL(glBindVertexArray(0));

  CALL(glDeleteVertexArrays(1, &objectVAO));
  CALL(glDeleteVertexArrays(1, &lightVAO));
  CALL(glDeleteBuffers(1, &VBO));

  objShdr.Dispose();
  lightSrcShdr.Dispose();

  return EXIT_SUCCESS;
}

void WindowErrorCallback(int error, const char* description)
{
  Logger::Instance(std::cerr).Log() << u8"[ERROR::GLFW] : code: " << error << " msg: " << description;
}

void UpdateCamera(Window& window)
{
  if (window.GetKeyState(GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera::MovementDirection::FORWARD, deltaTime);
  if (window.GetKeyState(GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera::MovementDirection::BACKWARD, deltaTime);
  if (window.GetKeyState(GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera::MovementDirection::RIGHT, deltaTime);
  if (window.GetKeyState(GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera::MovementDirection::LEFT, deltaTime);

  camera.ProcessMouseMovement(window.GetMouseXOffset(), window.GetMouseYOffset());

  camera.ProcessMouseScroll(window.GetScrollYOffset());
}

void UpdateLight(Window& window)
{
  if (window.GetKeyState(GLFW_KEY_UP) == GLFW_PRESS)
    lightPosition.z -= 2.5f * deltaTime;
  if (window.GetKeyState(GLFW_KEY_DOWN) == GLFW_PRESS)
    lightPosition.z += 2.5f * deltaTime;
  if (window.GetKeyState(GLFW_KEY_RIGHT) == GLFW_PRESS)
    lightPosition.x += 2.5f * deltaTime;
  if (window.GetKeyState(GLFW_KEY_LEFT) == GLFW_PRESS)
    lightPosition.x -= 2.5f * deltaTime;
  if (window.GetKeyState(GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    lightPosition.y += 2.5f * deltaTime;
  if (window.GetKeyState(GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    lightPosition.y -= 2.5f * deltaTime;
}
