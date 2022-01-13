#pragma once

#include <array>
#include <map>
#include <string>

#include <GL/glew.h>

class Shader {
public:
	~Shader();

	void LoadFromFile(unsigned int type, const char* fileName);
	void LoadFromString(unsigned int type, const std::string& source);

	void SetAttributeLocation(unsigned int index, const char* attribute);

	void Link();

	unsigned int Program() const;
	
	unsigned int GetAttributeLocation(const char* attribute);
	unsigned int GetUniformLocation(const char* uniform);

	void SetUniform1b(unsigned int uniformLocation, bool value);
	void SetUniform1i(unsigned int uniformLocation, int value);
	void SetUniform1f(unsigned int uniformLocation, float value);
  void SetUniform3f(unsigned int uniformLocation, float value1, float value2, float value3);
  void SetUniform3fv(unsigned int uniformLocation, const float* value, int count = 1);
	void SetUniformMatrix4fv(unsigned int uniformLocation, const float* value, unsigned char transpose = GL_FALSE);
	
	void Bind() const;
	void UnBind() const;

	void Dispose();

private:
	static constexpr unsigned int maxShaderCount = 4;

	unsigned int m_Program = 0;
	unsigned int m_ShaderCount = 0;
	std::array<unsigned int, maxShaderCount> m_Shaders{ 0, 0, 0, 0 };
	std::map<std::string, unsigned int> m_AttributeLocations{};
	std::map<std::string, unsigned int> m_UniformLocations{};
};

