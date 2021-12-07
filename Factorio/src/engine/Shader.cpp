#include "Shader.h"

#include <glew.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Error.h"

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath) :
    m_VertexFilePath(vertexFilePath),
    m_FragmentFilePath(fragmentFilePath),
    m_RendererID(0)
{
    std::string vertexShader = ParseShader(vertexFilePath);
    std::string fragmentShader = ParseShader(fragmentFilePath);
    m_RendererID = CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

std::string Shader::ParseShader(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::string line;
    std::stringstream ss;
    while (getline(file, line))
    {
        ss << line << '\n';
    }
    return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // error handling.
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << "shader.\n";
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) const  // reduces data transfers from GPU.
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' does not exist." << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}
