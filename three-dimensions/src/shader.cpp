#include "shader.h"

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path) {
    // Read source
    auto vertex_src = _read_source_file(vertex_path);
    auto frag_src = _read_source_file(fragment_path);

    // Compile them
    unsigned int vertex, frag;
    try {
        vertex = _compile_shader(vertex_src, GL_VERTEX_SHADER);
    }
    catch (const std::exception& e) {
        std::cout << "Failed to compile vertex shader\n" << e.what() << "\n";
        return;
    }

    try {
        frag = _compile_shader(frag_src, GL_FRAGMENT_SHADER);
    }
    catch (const std::exception& e) {
        std::cout << "Failed to compile fragment shader\n" << e.what() << "\n";
        return;
    }

    // Link them
    _id = glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, frag);
    glLinkProgram(_id);

    // Error checking
    int success;
    char info_log[INFO_LOG_SIZE];
    
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, INFO_LOG_SIZE, nullptr, info_log);
        std::cout << "Failed to link shader program" << "\n" << info_log << "\n";
    }

    // Delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(frag);
}

void Shader::use() {
    glUseProgram(_id);
}

void Shader::set_bool(const std::string& name, const bool& value) {
    set_int(name, static_cast<int>(value));
}

void Shader::set_int(const std::string& name, const int& value) {
    glUniform1i(_get_uniform_location(name), value);
}

void Shader::set_float(const std::string& name, const float& value) {
    glUniform1f(_get_uniform_location(name), value);
}

void Shader::set_vec2(const std::string& name, const float& x, const float& y) {
    glUniform2f(_get_uniform_location(name), x, y);
}

void Shader::set_vec2(const std::string& name, const glm::vec2& value) {
    glUniform2f(_get_uniform_location(name), value.x, value.y);
}

void Shader::set_vec3(const std::string& name, const float& x, const float& y, 
        const float& z) {
    glUniform3f(_get_uniform_location(name), x, y, z);
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value) {
    glUniform3f(_get_uniform_location(name), value.x, value.y, value.z);
}

void Shader::set_vec4(const std::string& name, const float& x, const float& y,
        const float& z, const float& w) {
    glUniform4f(_get_uniform_location(name), x, y, z, w);
}

void Shader::set_vec4(const std::string& name, const glm::vec4& value) {
    glUniform4f(_get_uniform_location(name), value.x, value.y, value.z, value.w);
}

void Shader::set_mat2(const std::string& name, const glm::mat2& value) {
    glUniformMatrix2fv(_get_uniform_location(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::set_mat3(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(_get_uniform_location(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::set_mat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(_get_uniform_location(name), 1, GL_FALSE, &value[0][0]);
}

const std::string Shader::_read_source_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open source file: " << path << "\n";
        return "";
    }

    std::string src, line;
    while (std::getline(file, line))
        src += line + "\n";

    file.close();
    
    return src;
}

const unsigned int Shader::_compile_shader(std::string& src, GLenum type) {
    auto src_cstr = src.c_str();
    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &src_cstr, nullptr);
    glCompileShader(shader);

    int success;
    char info_log[INFO_LOG_SIZE];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, INFO_LOG_SIZE, nullptr, info_log);
        throw std::invalid_argument(info_log);
    }

    return shader;
}

const unsigned int Shader::_get_uniform_location(const std::string& name) {
    return glGetUniformLocation(_id, name.c_str());
}
