#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
private:
	std::string m_VertexFilePath;
	std::string m_FragmentFilePath;
	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, unsigned int> m_UniformLocationCache; // mutable as we are updating this in a const function.
public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float value);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);
private:
	std::string ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name) const; // const as not editing uniform location.
};

