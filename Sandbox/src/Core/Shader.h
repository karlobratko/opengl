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

	void SetUniform1b(unsigned int uniformLocation, bool value) const;
	void SetUniform1i(unsigned int uniformLocation, int value) const;
	void SetUniform1f(unsigned int uniformLocation, float value) const;
	void SetUniformMatrix4fv(unsigned int uniformLocation, unsigned char transpose, const float* value);
	
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

