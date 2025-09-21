#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

static constexpr int INFO_LOG_SIZE = 512;

class Shader {
public:
    Shader(const std::string& vertex_path, const std::string& fragment_path);
    
    // Use the shader program
    void use();

    // Set uniforms
    void set_bool(const std::string& name, const bool& value);
    void set_int(const std::string& name, const int& value);
    void set_float(const std::string& name, const float& value);

    void set_vec2(const std::string& name, const float& x, const float& y);
    void set_vec3(const std::string& name, const float& x, const float& y,
            const float& z);
    void set_vec4(const std::string& name, const float& x, const float& y,
            const float& z, const float& w);
    void set_vec2(const std::string& name, const glm::vec2& value);
    void set_vec3(const std::string& name, const glm::vec3& value);
    void set_vec4(const std::string& name, const glm::vec4& value);

    void set_mat2(const std::string& name, const glm::mat2& value);
    void set_mat3(const std::string& name, const glm::mat3& value);
    void set_mat4(const std::string& name, const glm::mat4& value);

private:
    unsigned int _id;

    const std::string _read_source_file(const std::string& path);
    const unsigned int _compile_shader(std::string& src, GLenum type);
    const unsigned int _get_uniform_location(const std::string& name);
};
