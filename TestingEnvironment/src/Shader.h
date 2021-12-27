#pragma once

#include <GL/glew.h>
#include <array>
#include <map>
#include <string>

#include "IBindable.h"
#include "IDisposable.h"

using uint = unsigned int;

class Shader : public IBindable, public IDisposable {
public:
	virtual ~Shader();

	void LoadFromFile(uint type, const char* fileName);
	void LoadFromString(uint type, const std::string& source);

	void SetAttributeLocation(uint index, const char* attribute);

	void Link();

	uint Program() const;
	
	uint GetAttributeLocation(const char* attribute);
	uint GetUniformLocation(const char* uniform);

	void SetUniform1b(uint uniformLocation, bool value) const;
	void SetUniform1i(uint uniformLocation, int value) const;
	void SetUniform1f(uint uniformLocation, float value) const;
	void SetUniformMatrix4fv(uint uniformLocation, unsigned char transpose, const float* value);
	
	virtual void Bind() const override;
	virtual void UnBind() const override;

	virtual void Dispose() override;

private:
	static constexpr uint maxShaderCount = 4;

	uint m_Program = 0;
	uint m_ShaderCount = 0;
	std::array<uint, maxShaderCount> m_Shaders{ 0, 0, 0, 0 };
	std::map<std::string, uint> m_AttributeLocations{};
	std::map<std::string, uint> m_UniformLocations{};
};

