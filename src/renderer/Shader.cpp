#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <optional>

#include <glad/gl.h>
#include <fmt/core.h>

#include <Shader.hpp>
#include <Logger.hpp>
#include <WhirlError.hpp>

static void Compile(unsigned int shader, const char* source)
{
    WHIRL_TRACE("Compiling shader...");
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);

        std::vector<char> log(size);
        glGetShaderInfoLog(shader, size, nullptr, log.data());
        throw WhirlError("Failed to compile shader: \n{}", log.data());
    }
    else
    {
        WHIRL_TRACE("Shader compiled");
    }
}

static void Link(unsigned int program, unsigned int vShader, unsigned int fShader)
{
    WHIRL_TRACE("Linking shaders...");
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result)
    {
        int size;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);

        std::vector<char> log(size);
        glGetProgramInfoLog(program, size, nullptr, log.data());
        throw WhirlError("Failed to link shader program: \n{}", log.data());
    }
    else
    {
        WHIRL_TRACE("Shaders linked");
        glValidateProgram(program);
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    WHIRL_TRACE("Shaders deleted");
}

Shader::Shader(const std::string& path)
{
    WHIRL_TRACE("Reading shader file: {}", path);
    std::ifstream shaderFile(path);
    if (!shaderFile)
        throw WhirlError("Failed to open shader file: {}", path);

    std::string line;
    std::string vShaderCode;
    std::string fShaderCode;

    bool readingVertex = false;
    bool readingFragment = false;
    while (std::getline(shaderFile, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") == std::string::npos && line.find("fragment") == std::string::npos)
                throw WhirlError("Unknown shader tag found in: {}, -> {} <-", path, line);
        }

        if (line.find("#shader vertex") != std::string::npos)
        {
            if (readingFragment)
            {
                WHIRL_TRACE("Reading vertex shader source...");
                readingFragment = false;
                readingVertex = true;
                continue;
            }

            if (readingVertex)
                throw WhirlError("Unexpected shader tag found in: {}, -> {} <-", path, line);

            WHIRL_TRACE("Reading vertex shader source...");
            readingVertex = true;
            continue;
        }

        if (line.find("#shader fragment") != std::string::npos)
        {
            if (readingVertex)
            {
                WHIRL_TRACE("Reading fragment shader source...");
                readingVertex = false;
                readingFragment = true;
                continue;
            }

            if (readingFragment)
                throw WhirlError("Unknown shader tag found in: {}, -> {} <-", path, line);

            WHIRL_TRACE("Reading fragment shader source...");
            readingFragment = true;
            continue;
        }

        if (readingVertex)
        {
            if (!line.empty())
                vShaderCode.append(line + "\n");

            continue;
        }

        if (readingFragment)
        {
            if (!line.empty())
                fShaderCode.append(line + "\n");

            continue;
        }
    }

    if (vShaderCode.empty() || fShaderCode.empty())
        throw WhirlError("Missing shader code in: {}", path);

    WHIRL_TRACE("Shader source loaded successfully from file: {}", path);
    shaderFile.close();

    // Pass shaders to OpenGL
    m_handle = glCreateProgram();
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);

    try
    {
        Compile(vShader, vShaderCode.c_str());
        Compile(fShader, fShaderCode.c_str());
        Link(m_handle, vShader, fShader);
        WHIRL_TRACE("Shader constructed successfully from file: {}", path);
    }
    catch (WhirlError& error)
    {
        error.Context("Failed to construct shader");
        throw;
    }
}

Shader::~Shader()
{
    if (m_handle != 0)
    {
        WHIRL_DEBUG("Deleting shader program: {}", m_handle);
        glDeleteProgram(m_handle);
    }
}

void Shader::Use() const
{
    if (m_handle == 0)
    {
        // Consider throwing an exception here too
        WHIRL_ERROR("Tried to use an invalid shader");
        return;
    }

    // Use shader
    glUseProgram(m_handle);
}

static std::optional<int> GetLocation(uint32_t handle, const std::string& name)
{
    int location = glGetUniformLocation(handle, name.c_str());
    if (location == -1)
    {
        WHIRL_ERROR("Tried uploading a uniform to an unknown variable: {}", name);
        return std::nullopt;
    }

    return location;
}

bool Shader::SetBool(const std::string& name, bool value)
{
    auto location = GetLocation(m_handle, name);
    if (!location)
        return false;

    glUniform1i(*location, value);
    return true;
}

bool Shader::SetInt(const std::string& name, int value)
{
    auto location = GetLocation(m_handle, name);
    if (!location)
        return false;

    glUniform1i(*location, value);
    return true;
}

bool Shader::SetUInt(const std::string& name, unsigned int value)
{
    auto location = GetLocation(m_handle, name);
    if (!location)
        return false;

    glUniform1ui(*location, value);
    return true;
}

bool Shader::SetFloat(const std::string& name, float value)
{
    auto location = GetLocation(m_handle, name);
    if (!location)
        return false;

    glUniform1f(*location, value);
    return true;
}

bool Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
    auto location = GetLocation(m_handle, name);
    if (!location)
        return false;

    glUniformMatrix4fv(*location, 1, GL_FALSE, glm::value_ptr(matrix));
    return true;
}