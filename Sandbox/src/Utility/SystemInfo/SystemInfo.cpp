#include "SystemInfo.h"

#include <iostream>

#include <GL/glew.h>

#include "Core/Core.h"
#include "Logging/Logger.h"

void SystemInfo() {
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

  for (int i = 0; i < 10; ++i)
  {
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