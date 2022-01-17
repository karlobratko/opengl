#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GM_IMPLEMENTATION
#include "gm.h"

#define DEFAULT_WINDOW_WIDTH  1600
#define DEFAULT_WINDOW_HEIGHT 900
#define DEFAULT_WINDOW_TITLE "C OpenGL"
#define DEFAULT_MANUAL_TIME_STEP 0.1f
#define DEFAULT_SWAP_INTERVAL 1

#define VERTEX_SHADER_PATH   "res/shaders/main.vert"
#define FRAGMENT_SHADER_PATH "res/shaders/main.frag"

float time = 0.0f;
bool pause = false;

char* read_file_into_malloc_cstr(const char* file_path);

GLint program = 0;
bool program_failed = false;

const char* shader_type_to_cstr(GLenum type);
bool shader_compile_source(const GLchar* source, GLenum type, GLuint* id);
bool shader_compile_file(const char* file_path, GLenum type, GLuint* id);
bool shader_link_program(GLuint vert_shader, GLuint frag_shader, GLuint* id);
bool shader_load_program(const char* vert_file_path, const char* frag_file_path, GLuint* id);
void shader_reload(void);

typedef enum {
  UNIFORM_RESOLUTION = 0,
  UNIFORM_TIME,
  UNIFORM_MOUSE,
  COUNT_UNIFORMS
} uniform_e;

static_assert(COUNT_UNIFORMS == 3, "[ERROR] uniform_e enum was changed, update list of uniform names");
static const char* uniform_names[COUNT_UNIFORMS] = {
  [UNIFORM_RESOLUTION] = "resolution",
  [UNIFORM_TIME]       = "time",
  [UNIFORM_MOUSE]      = "mouse",
};
GLint uniform_locations[COUNT_UNIFORMS] = { 0 };

typedef enum {
  VERTEX_ATTRIBUTE_POSITION = 0,
  VERTEX_ATTRIBUTE_COLOR,
  VERTEX_ATTRIBUTE_NORMAL,
  VERTEX_ATTRIBUTE_TEXTURE_COORDINATE,
  COUNT_VERTEX_ATTRIBUTES
} vertex_attribute_e;

typedef struct {
  vec3f_t position;
  vec4f_t color;
  vec3f_t normal;
  vec2f_t tex_coord;
} vertex_t;

GLint VAO = 0;
GLint VBO = 0;
#define VERTEX_BUFFER_CAPACITY (1 * 1024) // vertices
vertex_t vertex_buffer[VERTEX_BUFFER_CAPACITY] = { 0 };
size_t vertex_buffer_size = 0;

void vertex_buffer_init(void);
void vertex_buffer_push_vertex(vec3f_t position, vec4f_t color, vec3f_t normal, vec2f_t tex_coord);
void vertex_buffer_push_quad(vec3f_t bottom_left, vec3f_t top_right, vec4f_t color, vec3f_t normal);
void vertex_buffer_sync(void);

int window_width  = DEFAULT_WINDOW_WIDTH;
int window_height = DEFAULT_WINDOW_HEIGHT;
GLFWwindow* window = NULL;

void window_error_callback(int error, const char* desc);
void window_key_callback(GLFWwindow* window, int key, int code, int action, int mode);
void window_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_calculate_fps();

void opengl_system_info(void);
void APIENTRY opengl_debug_message_callback(GLenum source, 
                                            GLenum type, 
                                            GLuint id, 
                                            GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* user_param);

int main(void) {
  printf("[INFO] GLFW vesion: %s\n", glfwGetVersionString());
  glfwSetErrorCallback(window_error_callback);
  if (!glfwInit())
  {
    fprintf(stderr, "[ERROR] Could not init GLFW.\n");
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  window = glfwCreateWindow(window_width, window_height, "OpenGL, C template", NULL, NULL);

  if (window == NULL)
  {
    fprintf(stderr, "[ERROR] Could not create a window.\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);
  glfwSetKeyCallback(window, window_key_callback);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(DEFAULT_SWAP_INTERVAL);

  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK)
  {
    glfwDestroyWindow(window);
    glfwTerminate();
    fprintf(stderr, "[ERROR] GLEW initialization failed\n%s\n", glewGetErrorString(GLEW_VERSION));
    return EXIT_FAILURE;
  }

  int opengl_version_major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
  int opengl_version_minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
  printf("[INFO] OpenGL %d.%d\n", opengl_version_major, opengl_version_minor);

  opengl_system_info();

  if (glDebugMessageCallback != NULL)
  {
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(opengl_debug_message_callback, NULL);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  vertex_buffer_init();
  vertex_buffer_push_quad(vec3f(-1.0f, -1.0f, 0.0f), vec3f(1.0f, 1.0f, 0.0f), vec4f(INT_TO_RGBA(0x00000011)), vec3f(0.0f, 0.0f, 0.0f));
  vertex_buffer_sync();

  shader_reload();

  time = glfwGetTime();
  float prev_time = 0.0f;
  while (!glfwWindowShouldClose(window))
  {
    window_calculate_fps();

    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!program_failed)
    {
      static_assert(COUNT_UNIFORMS == 3, "[ERROR] uniform_e enum was changed, update the uniform sync");

      glUniform2f(uniform_locations[UNIFORM_RESOLUTION], (float)window_width, (float)window_height);
      glUniform1f(uniform_locations[UNIFORM_TIME], time);

      double xpos = 0, ypos = 0;
      glfwGetCursorPos(window, &xpos, &ypos);
      glUniform2f(uniform_locations[UNIFORM_MOUSE], (float)xpos, (float) (window_height - ypos));

      glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_size);
    }


    float cur_time = glfwGetTime();
    if (!pause)
    {
      time += cur_time - prev_time;
    }
    prev_time = cur_time;

    glfwSwapBuffers(window);
  }

  return EXIT_SUCCESS;
}

char* read_file_into_malloc_cstr(const char* file_path)
{
  FILE* handle = NULL;
  char* buffer = NULL;

  handle = fopen(file_path, "rb");
  if (handle == NULL) goto fail;
  if (fseek(handle, 0, SEEK_END) != 0) goto fail;

  long size = ftell(handle);
  if (size == -1) goto fail;

  buffer = malloc(sizeof(char) * (size + 1));
  if (buffer == NULL) goto fail;

  if (fseek(handle, 0, SEEK_SET) != 0) goto fail;

  if (fread(buffer, sizeof(char), size, handle) != size) goto fail;

  buffer[size] = '\0';

  if (handle != NULL) 
  {
    fclose(handle);
    errno = 0;
  }

  return buffer;

fail:
  if (handle != NULL) 
  {
    int saved_errno = errno;
    fclose(handle);
    errno = saved_errno;
  }
  if (buffer != NULL)
  {
    free(buffer);
  }

  return NULL;
}

const char* shader_type_to_cstr(GLenum type)
{
  switch (type)
  {
  case GL_VERTEX_SHADER:
    return "GL_VERTEX_SHADER";
  case GL_FRAGMENT_SHADER:
    return "GL_FRAGMENT_SHADER";
  default:
    return "UNDEFINED";
  }
}

bool shader_compile_source(const GLchar* source, GLenum type, GLuint* id)
{
  *id = glCreateShader(type);
  glShaderSource(*id, 1, &source, NULL);
  glCompileShader(*id);

  GLint compile_status = 0;
  glGetShaderiv(*id, GL_COMPILE_STATUS, &compile_status);
  if (compile_status == GL_FALSE) 
  {
    GLint info_log_length = 0;
    glGetShaderiv(*id, GL_INFO_LOG_LENGTH, &info_log_length);

    GLchar* info_log = malloc(sizeof(char) * info_log_length);
    glGetShaderInfoLog(*id, info_log_length, NULL, info_log);

    fprintf(stderr, "[ERROR] Could not compile %s\n", shader_type_to_cstr(type));
    fprintf(stderr, "%.*s\n", info_log_length, info_log);

    free(info_log);
    return false;
  }

  return true;
}

bool shader_compile_file(const char* file_path, GLenum type, GLuint* id)
{
  char* source = read_file_into_malloc_cstr(file_path);

  if (source == NULL) 
  {
    fprintf(stderr, "[ERROR] Failed to read file '%s' : %s\n", file_path, strerror(errno));
    errno = 0;
    return false;
  }

  bool is_compiled = shader_compile_source(source, type, id);
  if (!is_compiled) 
  {
    fprintf(stderr, "[ERROR] Failed to compile '%s' shader file\n", file_path);
  }

  free(source);
  return is_compiled;
}

bool shader_link_program(GLuint vert_shader, GLuint frag_shader, GLuint* id)
{
  *id = glCreateProgram();
  glAttachShader(*id, vert_shader);
  glAttachShader(*id, frag_shader);

  GLint link_status = 0;
  glLinkProgram(*id);
  glGetProgramiv(*id, GL_LINK_STATUS, &link_status);
  if (link_status == GL_FALSE)
  {
    GLint info_log_length = 0;
    glGetProgramiv(*id, GL_INFO_LOG_LENGTH, &info_log_length);

    GLchar* info_log = malloc(sizeof(char) * info_log_length);
    glGetProgramInfoLog(*id, info_log_length, NULL, info_log);

    fprintf(stderr, "[ERROR] Could not link program.\n%.*s\n", info_log_length, info_log);

    free(info_log);
    return false;
  }

  GLint validate_status = 0;
  glValidateProgram(*id);
  glGetProgramiv(*id, GL_VALIDATE_STATUS, &validate_status);
  if (validate_status == GL_FALSE) 
  {
    GLint info_log_length = 0;
    glGetProgramiv(*id, GL_INFO_LOG_LENGTH, &info_log_length);

    GLchar* info_log = malloc(sizeof(char) * info_log_length);
    glGetProgramInfoLog(*id, info_log_length, NULL, info_log);

    fprintf(stderr, "[ERROR] Could not link program.\n%.*s\n", info_log_length, info_log);

    free(info_log);
    return false;
  }

  glDetachShader(*id, vert_shader);
  glDeleteShader(vert_shader);
  glDetachShader(*id, frag_shader);
  glDeleteShader(frag_shader);

  return true;
}

bool shader_load_program(const char* vert_file_path, const char* frag_file_path, GLuint* id)
{
  GLuint vert_shader = 0;
  if (!shader_compile_file(vert_file_path, GL_VERTEX_SHADER, &vert_shader)) 
  {
    return false;
  }

  GLuint frag_shader = 0;
  if (!shader_compile_file(frag_file_path, GL_FRAGMENT_SHADER, &frag_shader))
  {
    return false;
  }

  if (!shader_link_program(vert_shader, frag_shader, id))
  {
    return false;
  }

  return true;
}

void shader_reload(void)
{
  if (program)
  {
    glDeleteProgram(program);
    program = 0;
  }
  program_failed = true;
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

  if (!shader_load_program(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH, &program)) 
  {
    return;
  }

  glUseProgram(program);

  for (uniform_e i = 0; i < COUNT_UNIFORMS; ++i) 
  {
    const char* name = uniform_names[i];
    uniform_locations[i] = glGetUniformLocation(program, uniform_names[i]);
  }

  program_failed = false;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  printf("[INFO] Shaders reloaded successfully.\n");
}

void vertex_buffer_init(void)
{
  if (VAO)
  {
    return;
  }

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), vertex_buffer, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(VERTEX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, position));
  glEnableVertexAttribArray(VERTEX_ATTRIBUTE_POSITION);
  glVertexAttribPointer(VERTEX_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, color));
  glEnableVertexAttribArray(VERTEX_ATTRIBUTE_COLOR);
  glVertexAttribPointer(VERTEX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, normal));
  glEnableVertexAttribArray(VERTEX_ATTRIBUTE_NORMAL);
  glVertexAttribPointer(VERTEX_ATTRIBUTE_TEXTURE_COORDINATE, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void*)offsetof(vertex_t, tex_coord));
  glEnableVertexAttribArray(VERTEX_ATTRIBUTE_TEXTURE_COORDINATE);
}

void vertex_buffer_push_vertex(vec3f_t position, vec4f_t color, vec3f_t normal, vec2f_t tex_coord)
{
  assert(vertex_buffer_size < VERTEX_BUFFER_CAPACITY);
  vertex_buffer[vertex_buffer_size++] = (vertex_t) { position, color, normal, tex_coord };
}

void vertex_buffer_push_quad(vec3f_t bottom_left, vec3f_t top_right, vec4f_t color, vec3f_t normal)
{
  vec3f_t top_left     = vec3f(bottom_left.x, top_right.y, bottom_left.z);
  vec3f_t bottom_right = vec3f(top_right.x, bottom_left.y, top_right.z);

  vertex_buffer_push_vertex(bottom_left, color, normal, vec2f(0.0f, 0.0f));
  vertex_buffer_push_vertex(top_left, color, normal, vec2f(0.0f, 1.0f));
  vertex_buffer_push_vertex(top_right, color, normal, vec2f(1.0f, 1.0f));

  vertex_buffer_push_vertex(top_right, color, normal, vec2f(1.0f, 1.0f));
  vertex_buffer_push_vertex(bottom_right, color, normal, vec2f(1.0f, 0.0f));
  vertex_buffer_push_vertex(bottom_left, color, normal, vec2f(0.0f, 0.0f));
}


void vertex_buffer_sync(void)
{
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_t) * vertex_buffer_size, vertex_buffer);
}

void window_error_callback(int error, const char* desc)
{
  fprintf(stderr, "[ERROR] GLFW error code %d, %s\n", error, desc);
}

void window_key_callback(GLFWwindow* window, int key, int code, int action, int mode)
{
  (void)code;
  (void)mode;

  if (action == GLFW_PRESS) 
  {
    if (key == GLFW_KEY_ESCAPE)
    {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_F5)
    {
      shader_reload();
    }
    if (key == GLFW_KEY_SPACE)
    {
      pause = !pause;
    }

    if (pause)
    {
      if (key == GLFW_KEY_LEFT)
      {
        time -= DEFAULT_MANUAL_TIME_STEP;
      }
      else if (key == GLFW_KEY_RIGHT)
      {
        time += DEFAULT_MANUAL_TIME_STEP;
      }
    }
  }
}

void window_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  (void)window;
  window_width = width;
  window_height = height;
  glViewport(0, 0, width, height);
}

void window_calculate_fps()
{
  static float prev_sec = 0.0f;
  static int frame_count;

  float cur_sec = glfwGetTime();
  float delta_sec = cur_sec - prev_sec;

  if (delta_sec > 0.25)
  {
    prev_sec = cur_sec;
    float fps = (float)frame_count / delta_sec;

    char title[128];
    sprintf(title, DEFAULT_WINDOW_TITLE" @ fps: %.2f", fps);
    glfwSetWindowTitle(window, title);

    frame_count = 0;
  }

  ++frame_count;
}

void opengl_system_info(void)
{
  GLuint system_param_enums[] = {
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

  const char* system_param_names[] = {
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

  for (int i = 0; i < 10; ++i)
  {
    int value = 0;
    glGetIntegerv(system_param_enums[i], &value);
    printf("[INFO] %s : %d\n", system_param_names[i], value);
  }

  int value[2] = { 0, 0 };
  glGetIntegerv(system_param_enums[10], value);
  printf("[INFO] %s : [%d, %d]\n", system_param_names[10], value[0], value[1]);

  GLboolean flag = 0;
  glGetBooleanv(system_param_enums[11], &flag);
  printf("[INFO] %s : %d\n", system_param_names[11], (flag > 0 ? 1 : 0));
}

void APIENTRY opengl_debug_message_callback(GLenum source, 
                                            GLenum type, 
                                            GLuint id, 
                                            GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* user_param)
{
  (void)user_param;
  (void)length;
  fprintf(stderr,
          "[%s] source: 0x%x, type: 0x%x, id: %d, severity: 0x%x, message: %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "ERROR" : "INFO"), 
          source, type, id, severity, message);
}
