#include "Shader.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "Core/Core.h"

Shader::~Shader()
{
  Shader::Dispose();
}

void Shader::LoadFromFile(unsigned int type, const char* fileName) 
{
  std::ifstream file{ fileName };
  if (file.is_open()) 
  {
    std::string buffer{ std::istreambuf_iterator<char>{ file }, {} };
    LoadFromString(type, buffer);
    file.close();
  }
  else 
  {
    throw std::exception("[ERROR::SHADER] : could not access file");
  }
}

void Shader::LoadFromString(unsigned int type, const std::string& source) 
{
  CALL(unsigned int shader = glCreateShader(type));
  const char* csource = source.c_str();
  CALL(glShaderSource(shader, 1, &csource, nullptr));

  CALL(glCompileShader(shader));
  int compileStatus = 0;
  CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus));
  if (compileStatus == GL_FALSE) 
  {
    int infoLogSize;
    CALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize));

    char* infoLog = new char[infoLogSize];
    CALL(glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog));

    std::ostringstream outStream{};
    outStream << "[ERROR::SHADER] : error compiling the " << type << " shader\n" << infoLog;

    delete[] infoLog;
    throw std::exception{ outStream.str().c_str() };
  }

  m_Shaders[m_ShaderCount++] = shader;
}

void Shader::SetAttributeLocation(unsigned int index, const char* attribute) 
{
  CALL(glBindAttribLocation(m_Program, index, attribute));

  if (m_AttributeLocations.find(attribute) == std::end(m_AttributeLocations)) 
  {
    m_AttributeLocations.emplace(attribute, index);
  }
}

void Shader::Link() 
{
  CALL(m_Program = glCreateProgram());

  std::for_each(
    std::begin(m_Shaders),
    std::end(m_Shaders),
    [&](unsigned int id) 
    {
      if (id) 
      {
        CALL(glAttachShader(m_Program, id));
      }
    }
  );

  { int linkStatus = 0;
    CALL(glLinkProgram(m_Program));
    CALL(glGetProgramiv(m_Program, GL_LINK_STATUS, &linkStatus));
    if (linkStatus == GL_FALSE) 
    {
      int infoLogSize;
      CALL(glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infoLogSize));

      char* infoLog = new char[infoLogSize];
      CALL(glGetProgramInfoLog(m_Program, infoLogSize, nullptr, infoLog));

      std::ostringstream outStream{};
      outStream << "[ERROR::SHADER] : error linking " << m_Program << " program\n" << infoLog;

      delete[] infoLog;
      throw std::exception{ outStream.str().c_str() };
    }
  }

  { int validationStatus = 0;
    CALL(glValidateProgram(m_Program));
    CALL(glGetProgramiv(m_Program, GL_VALIDATE_STATUS, &validationStatus));
    if (validationStatus == GL_FALSE) 
    {
      int infoLogSize;
      CALL(glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infoLogSize));

      char* infoLog = new char[infoLogSize];
      CALL(glGetProgramInfoLog(m_Program, infoLogSize, nullptr, infoLog));

      std::ostringstream outStream{};
      outStream << "[ERROR::SHADER] : error linking " << m_Program << " program\n" << infoLog;

      delete[] infoLog;
      throw std::exception{ outStream.str().c_str() };
    }
  }

  std::for_each(
    std::begin(m_Shaders),
    std::end(m_Shaders),
    [&](unsigned int id) 
    {
      if (id) 
      {
        CALL(glDetachShader(m_Program, id));
        CALL(glDeleteShader(id));
      }
    }
  );
}

unsigned int Shader::Program() const 
{
  return m_Program;
}

unsigned int Shader::GetAttributeLocation(const char* attribute) 
{
  auto it = m_AttributeLocations.find(attribute);
  if (it != std::end(m_AttributeLocations)) 
  {
    return it->second;
  }

  CALL(int attributeLocation = glGetAttribLocation(m_Program, attribute));
  if (attributeLocation == -1) 
  {
    std::ostringstream outStream{};
    outStream << "[ERROR::SHADER] : could not find attribute " << attribute;
    throw std::exception{ outStream.str().c_str() };
  }

  m_AttributeLocations.emplace(attribute, attributeLocation);
  return attributeLocation;
}

unsigned int Shader::GetUniformLocation(const char* uniform) 
{
  auto it = m_UniformLocations.find(uniform);
  if (it != std::end(m_UniformLocations)) 
  {
    return it->second;
  }

  CALL(int uniformLocation = glGetUniformLocation(m_Program, uniform));
  if (uniformLocation == -1) 
  {
    std::ostringstream outStream{};
    outStream << "[ERROR::SHADER] : could not find uniform  " << uniform;
    throw std::exception{ outStream.str().c_str() };
  }

  m_UniformLocations.emplace(uniform, uniformLocation);
  return uniformLocation;
}

void Shader::SetUniform1b(unsigned int uniformLocation, bool value) const 
{
  CALL(glUniform1i(uniformLocation, (int)value));
}
void Shader::SetUniform1i(unsigned int uniformLocation, int value) const 
{
  CALL(glUniform1i(uniformLocation, value));
}
void Shader::SetUniform1f(unsigned int uniformLocation, float value) const 
{
  CALL(glUniform1f(uniformLocation, value));
}
void Shader::SetUniform3f(unsigned int uniformLocation, float value1, float value2, float value3) const
{
  CALL(glUniform3f(uniformLocation, value1, value2, value3));
}
void Shader::SetUniform3fv(unsigned int uniformLocation, const float* value, int count)
{
  CALL(glUniform3fv(uniformLocation, count, value));
}
void Shader::SetUniformMatrix4fv(unsigned int uniformLocation, const float* value, unsigned char transpose)
{
  CALL(glUniformMatrix4fv(uniformLocation, 1, transpose, value));
}

void Shader::Bind() const 
{
  CALL(glUseProgram(m_Program));
}

void Shader::UnBind() const 
{
  CALL(glUseProgram(0));
}

void Shader::Dispose() 
{
  if (m_Program) 
  {
    CALL(glDeleteProgram(m_Program));
    m_Program = 0;
  }
}