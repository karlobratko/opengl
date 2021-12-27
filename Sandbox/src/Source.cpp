#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Core/Core.h"
#include "Core/Window.h"

#include "Logging/Logger.h"

// WINDOW
constexpr const char* WINDOW_NAME   = "Sandbox";
constexpr int         WINDOW_WIDTH  = 800;
constexpr int         WINDOW_HEIGHT = 600;

void window_error_callback(int error, const char* description);
void window_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_process_input(GLFWwindow* window);

void LogSystemInfo();

constexpr const char* vertexShader = R"(
  #version 330
  layout (location = 0) in vec3 position;
  void main() {
    gl_Position = vec4(position, 1.0f);
  }
)";

constexpr const char* fragmentShader = R"(
  #version 330
  out vec4 color;
  void main() {
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
  }
)";

int main() {
  Window window{ WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, false, window_framebuffer_size_callback, window_error_callback };
  try {
    window.SetHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window.SetHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window.SetHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window.SetHint(GLFW_SAMPLES, 4);

    window.SetProcessInputCallback(window_process_input);

    window.Initialize();
  }
  catch (const std::exception& err) {
    Logger::Instance(std::cerr).Log() << err.what();
    return 1;
  }
  LogSystemInfo();

  GLuint VAO, VBO;
  {
    GLfloat vertices[] = {
      -0.5f, -0.5f,  0.0f,
       0.0f,  0.5f,  0.0f,
       0.5f, -0.5f,  0.0f
    };

    CALL(glGenVertexArrays(1, &VAO));
    CALL(glGenBuffers(1, &VBO));

    CALL(glBindVertexArray(VAO));

    CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    
    CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<const void*>(0 * sizeof(float))));
    glEnableVertexAttribArray(0);

    CALL(glBindVertexArray(0));
    CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }

  GLuint program;
  {
    CALL(GLuint vShdr = glCreateShader(GL_VERTEX_SHADER));
    CALL(glShaderSource(vShdr, 1, &vertexShader, nullptr));
    CALL(glCompileShader(vShdr));
    int compileStatus = 0;
    CALL(glGetShaderiv(vShdr, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus == GL_FALSE) {
      int infoLogSize = 0;
      CALL(glGetShaderiv(vShdr, GL_INFO_LOG_LENGTH, &infoLogSize));
      char* infoLog = new char[infoLogSize];
      CALL(glGetShaderInfoLog(vShdr, infoLogSize, nullptr, infoLog));
      Logger::Instance(std::cerr).Log() << infoLog;
      delete[] infoLog;
      return EXIT_FAILURE;
    }

    CALL(GLuint fShdr = glCreateShader(GL_FRAGMENT_SHADER));
    CALL(glShaderSource(fShdr, 1, &fragmentShader, nullptr));
    CALL(glCompileShader(fShdr));
    compileStatus = 0;
    CALL(glGetShaderiv(fShdr, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus == GL_FALSE) {
      int infoLogSize = 0;
      CALL(glGetShaderiv(fShdr, GL_INFO_LOG_LENGTH, &infoLogSize));
      char* infoLog = new char[infoLogSize];
      CALL(glGetShaderInfoLog(fShdr, infoLogSize, nullptr, infoLog));
      Logger::Instance(std::cerr).Log() << infoLog;
      delete[] infoLog;
      return EXIT_FAILURE;
    }

    CALL(program = glCreateProgram());

    CALL(glAttachShader(program, vShdr));
    CALL(glAttachShader(program, fShdr));

    CALL(glLinkProgram(program));
    int linkStatus = 0;
    CALL(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));
    if (linkStatus == GL_FALSE) {
      int infoLogSize = 0;
      CALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogSize));
      char* infoLog = new char[infoLogSize];
      CALL(glGetProgramInfoLog(program, infoLogSize, nullptr, infoLog));
     Logger::Instance(std::cerr).Log() << infoLog;
      delete[] infoLog;
      return EXIT_FAILURE;
    }

    CALL(glValidateProgram(program));
    int validationStatus = 0;
    CALL(glGetProgramiv(program, GL_VALIDATE_STATUS, &validationStatus));
    if (validationStatus == GL_FALSE) {
      int infoLogSize = 0;
      CALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogSize));
      char* infoLog = new char[infoLogSize];
      CALL(glGetProgramInfoLog(program, infoLogSize, nullptr, infoLog));
     Logger::Instance(std::cerr).Log() << infoLog;
      delete[] infoLog;
      return EXIT_FAILURE;
    }

    CALL(glDetachShader(program, vShdr));
    CALL(glDeleteShader(vShdr));
    CALL(glDetachShader(program, fShdr));
    CALL(glDeleteShader(fShdr));
  }

  CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  while (!window.ShouldClose()) {
    window.CalculateFPS();
    glfwPollEvents();
    window.ProcessInput();

    CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    CALL(glClear(GL_COLOR_BUFFER_BIT));

    CALL(glUseProgram(program));
    CALL(glBindVertexArray(VAO));
    CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
    CALL(glBindVertexArray(0));
    CALL(glUseProgram(0));

    window.SwapBuffers();
  }

  CALL(glBindVertexArray(VAO));
  CALL(glDisableVertexAttribArray(0));
  CALL(glBindVertexArray(0));

  CALL(glDeleteVertexArrays(1, &VAO));
  CALL(glDeleteBuffers(1, &VBO));

  CALL(glDeleteProgram(program));

  return 0;
}

void window_error_callback(int error, const char* description) {
  Logger::Instance(std::cerr).Log() << u8"[ERROR::GLFW] : code: " << error << " msg: " << description;
}

void window_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  CALL(glViewport(0, 0, width, height));
}

void window_process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void LogSystemInfo() {
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
    "GL_MAX_CUBE_MAP_TEXTURE_SIZE       ",
    "GL_MAX_DRAW_BUFFERS                ",
    "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS ",
    "GL_MAX_TEXTURE_IMAGE_UNITS         ",
    "GL_MAX_TEXTURE_SIZE                ",
    "GL_MAX_VARYING_FLOATS              ",
    "GL_MAX_VERTEX_ATTRIBS              ",
    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS  ",
    "GL_MAX_VERTEX_UNIFORM_COMPONENTS   ",
    "GL_MAX_VIEWPORT_DIMS               ",
    "GL_STEREO                          "
  };

  for (int i = 0; i < 10; ++i) {
    int v = 0;
    CALL(glGetIntegerv(params[i], &v));
    Logger::Instance(std::cout).Log() << "[INFO::GL] " << string_rep[i] << " : " << v;
  }

  int v[2] = { 0, 0 };
  CALL(glGetIntegerv(params[10], v));
  Logger::Instance(std::cout).Log() << "[INFO::GL] " << string_rep[10] << " : [" << v[0] << ", " << v[1] << "]";

  unsigned char flag = 0;
  CALL(glGetBooleanv(params[11], &flag));
  Logger::Instance(std::cout).Log() << "[INFO::GL] " << string_rep[11] << " : " << (flag > 0 ? 1 : 0);
}