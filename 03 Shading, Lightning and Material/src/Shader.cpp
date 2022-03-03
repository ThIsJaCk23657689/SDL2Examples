#include "Shader.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path) {
    GLuint vertex = CreateShader(vertex_path, ShaderType::Vert);
    GLuint fragment = CreateShader(fragment_path, ShaderType::Frag);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);

    if (LinkShaderProgram(m_id) != GL_TRUE) {
        GLint len;
        std::string log;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);
        log.resize(len);
        glGetProgramInfoLog(m_id, len, nullptr, log.data());
        std::cerr << "[Error] " << log << std::endl;
        exit(-1);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(m_id);
}

void Shader::Use() const {
    glUseProgram(m_id);
}

void Shader::SetInt(const std::string& uniform_name, int value) {
    glUniform1i(GetUniformLocation(uniform_name), value);
}

void Shader::SetBool(const std::string& uniform_name, bool value) {
    glUniform1i(GetUniformLocation(uniform_name), value);
}

void Shader::SetFloat(const std::string& uniform_name, float value) {
    glUniform1f(GetUniformLocation(uniform_name), value);
}

void Shader::SetVec2(const std::string& uniform_name, const glm::vec2& vector) {
    glUniform2fv(GetUniformLocation(uniform_name), 1, glm::value_ptr(vector));
}

void Shader::SetVec3(const std::string& uniform_name, const glm::vec3& vector) {
    glUniform3fv(GetUniformLocation(uniform_name), 1, &vector[0]);
}

void Shader::SetVec4(const std::string& uniform_name, const glm::vec4& vector) {
    glUniform4fv(GetUniformLocation(uniform_name), 1, glm::value_ptr(vector));
}

void Shader::SetMat4(const std::string& uniform_name, const glm::mat4& matrix) {
    // glUniformMatrix4fv(GLuint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    // 第三個參數決定是否要轉置該矩陣
    glUniformMatrix4fv(GetUniformLocation(uniform_name), 1, GL_FALSE, glm::value_ptr(matrix));
}

GLuint Shader::CreateShader(const std::string& shader_filepath, ShaderType shader_type) {
    std::ifstream file;
    std::string source = "";

    file.open(shader_filepath);
    if (file.fail()) {
        std::cerr << "Failed to read shader file: \"" << shader_filepath << "\"." << std::endl;
        exit(1);
    }
    source.resize(std::filesystem::file_size(shader_filepath));
    file.read(source.data(), source.size());
    file.close();

    const char* ShaderCode = source.c_str();

    // Compile these shaders.
    GLuint shader_obj = glCreateShader(shader_type);
    glShaderSource(shader_obj, 1, &ShaderCode, nullptr);
    glCompileShader(shader_obj);

    if (CompileShader(shader_obj) != GL_TRUE) {
        GLint len;
        std::string log;
        glGetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &len);
        log.resize(len);
        glGetShaderInfoLog(shader_obj, len, nullptr, log.data());
        std::cerr << "[Error] On: " << shader_filepath << ": " << log << std::endl;
        exit(-2);
    }

    return shader_obj;
}

GLboolean Shader::CompileShader(const GLuint& shader_id) {
    GLint status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    return status;
}

GLboolean Shader::LinkShaderProgram(const GLuint& program_id) {
    GLint status;
    glGetProgramiv(program_id, GL_LINK_STATUS, &status);
    return status;
}

GLuint Shader::GetUniformLocation(const std::string& uniform_name) {
    if (m_uniform_location_cache.find(uniform_name) != m_uniform_location_cache.end()) {
        return m_uniform_location_cache[uniform_name];
    }

    GLuint location = glGetUniformLocation(m_id, uniform_name.c_str());
    if (location == -1) {
        std::cerr << "The uniform variable <" << uniform_name
                  << "> doesn't exist in this shader ID: " << std::to_string(m_id) << std::endl;
    }

    m_uniform_location_cache[uniform_name] = location;
    return location;
}