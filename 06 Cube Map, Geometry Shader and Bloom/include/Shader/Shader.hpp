#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <memory>
#include <unordered_map>

#include "Camera.hpp"

enum ShaderType : GLenum {
    Vert = GL_VERTEX_SHADER,
    Tesc = GL_TESS_CONTROL_SHADER,
    Tese = GL_TESS_EVALUATION_SHADER,
    Geom = GL_GEOMETRY_SHADER,
    Frag = GL_FRAGMENT_SHADER,
    Comp = GL_COMPUTE_SHADER,
};

struct Shader {
    Shader(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path = "");

    void Start() const;
    void Stop() const;
    void Destroy() const;

    void SetInt(const std::string& uniform_name, int value);
    void SetBool(const std::string& uniform_name, bool value);
    void SetFloat(const std::string& uniform_name, float value);
    void SetVec2(const std::string& uniform_name, const glm::vec2& vector);
    void SetVec3(const std::string& uniform_name, const glm::vec3& vector);
    void SetVec4(const std::string& uniform_name, const glm::vec4& vector);
    void SetMat4(const std::string& uniform_name, const glm::mat4& matrix);

    void SetViewAndProj(const std::unique_ptr<Camera>& camera);

protected:
    GLuint id;
    std::unordered_map<std::string, GLint> uniform_location_cache;

    GLint GetUniformLocation(const std::string& uniform_name);

private:
    GLuint vertex_id;
    GLuint fragment_id;
    GLuint geometry_id;

    GLuint CreateShader(const std::string& shader_filepath, ShaderType shader_type);
    GLboolean CompileShader(const GLuint& shader_id);
    GLboolean LinkShaderProgram();
};

#endif