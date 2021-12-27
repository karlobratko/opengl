#include "Shader.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include "gl_utils.h"
#include "Logger.h"

Shader::~Shader() {
	Shader::Dispose();
}

void Shader::LoadFromFile(uint type, const char* fileName) {
	std::ifstream file{ fileName };
	if (file.is_open()) {
		std::string buffer{ std::istreambuf_iterator<char>{ file }, {} };
		LoadFromString(type, buffer);
		file.close();
	}
	else {
		throw std::exception("[ERROR::SHADER] : could not access file");
	}
}

void Shader::LoadFromString(uint type, const std::string& source) {
	glCall(uint shader = glCreateShader(type));
	const char* csource = source.c_str();
	glCall(glShaderSource(shader, 1, &csource, nullptr));

	glCall(glCompileShader(shader));
	int compileStatus = 0;
	glCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus));
	if (compileStatus == GL_FALSE) {
		int infoLogSize;
		glCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize));

		char* infoLog = new char[infoLogSize];
		glCall(glGetShaderInfoLog(shader, infoLogSize, nullptr, infoLog));

		std::ostringstream outStream{};
		outStream << "[ERROR::SHADER] : error compiling the " << type << " shader\n" << infoLog;

		delete[] infoLog;
		throw std::exception{ outStream.str().c_str() };
	}
	
	m_Shaders[m_ShaderCount++] = shader;
}

void Shader::SetAttributeLocation(uint index, const char* attribute) {
	glCall(glBindAttribLocation(m_Program, index, attribute));

	if (m_AttributeLocations.find(attribute) == std::end(m_AttributeLocations)) {
		m_AttributeLocations.emplace(attribute, index);
	}
}

void Shader::Link() {
	glCall(m_Program = glCreateProgram());

	std::for_each(
		std::begin(m_Shaders),
		std::end(m_Shaders),
		[&](uint id) {
			if (id) {
				glCall(glAttachShader(m_Program, id));
			}
		}
	);

	{ int linkStatus = 0;
		glCall(glLinkProgram(m_Program));
		glCall(glGetProgramiv(m_Program, GL_LINK_STATUS, &linkStatus));
		if (linkStatus == GL_FALSE) {
			int infoLogSize;
			glCall(glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infoLogSize));

			char* infoLog = new char[infoLogSize];
			glCall(glGetProgramInfoLog(m_Program, infoLogSize, nullptr, infoLog));

			std::ostringstream outStream{};
			outStream << "[ERROR::SHADER] : error linking " << m_Program << " program\n" << infoLog;

			delete[] infoLog;
			throw std::exception{ outStream.str().c_str() };
		}
	}

	{ int validationStatus = 0;
		glCall(glValidateProgram(m_Program));
		glCall(glGetProgramiv(m_Program, GL_VALIDATE_STATUS, &validationStatus));
		if (validationStatus == GL_FALSE) {
			int infoLogSize;
			glCall(glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infoLogSize));

			char* infoLog = new char[infoLogSize];
			glCall(glGetProgramInfoLog(m_Program, infoLogSize, nullptr, infoLog));

			std::ostringstream outStream{};
			outStream << "[ERROR::SHADER] : error linking " << m_Program << " program\n" << infoLog;

			delete[] infoLog;
			throw std::exception{ outStream.str().c_str() };
		}
	}

	std::for_each(
		std::begin(m_Shaders),
		std::end(m_Shaders),
		[&](uint id) {
			if (id) {
				glCall(glDetachShader(m_Program, id));
				glCall(glDeleteShader(id));
			}
		}
	);
}

uint Shader::Program() const {
	return m_Program;
}

uint Shader::GetAttributeLocation(const char* attribute) {
	auto it = m_AttributeLocations.find(attribute);
	if (it != std::end(m_AttributeLocations)) {
		return it->second;
	}

	glCall(int attributeLocation = glGetAttribLocation(m_Program, attribute));
	if (attributeLocation == -1) {
		std::ostringstream outStream{};
		outStream << "[ERROR::SHADER] : could not find attribute " << attribute;
		throw std::exception{ outStream.str().c_str() };
	}

	m_AttributeLocations.emplace(attribute, attributeLocation);
	return attributeLocation;
}

uint Shader::GetUniformLocation(const char* uniform) {
	auto it = m_UniformLocations.find(uniform);
	if (it != std::end(m_UniformLocations)) {
		return it->second;
	}

	glCall(int uniformLocation = glGetUniformLocation(m_Program, uniform));
	if (uniformLocation == -1) {
		std::ostringstream outStream{};
		outStream << "[ERROR::SHADER] : could not find uniform  " << uniform;
		throw std::exception{ outStream.str().c_str() };
	}

	m_UniformLocations.emplace(uniform, uniformLocation);
	return uniformLocation;
}

void Shader::SetUniform1b(uint uniformLocation, bool value) const {
	glCall(glUniform1i(uniformLocation, (int)value));
}
void Shader::SetUniform1i(uint uniformLocation, int value) const {
	glCall(glUniform1i(uniformLocation, value));
}
void Shader::SetUniform1f(uint uniformLocation, float value) const {
	glCall(glUniform1f(uniformLocation, value));
}
void Shader::SetUniformMatrix4fv(uint uniformLocation, unsigned char transpose, const float* value) {
	glCall(glUniformMatrix4fv(uniformLocation, 1, transpose, value));
}

void Shader::Bind() const {
	glCall(glUseProgram(m_Program));
}

void Shader::UnBind() const {
	glCall(glUseProgram(0));
}

void Shader::Dispose() {
	if (m_Program) {
		glCall(glDeleteProgram(m_Program));
		m_Program = 0;
	}
}