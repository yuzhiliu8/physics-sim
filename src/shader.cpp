#include "shader.hpp"

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path){

    std::ifstream vertex_stream = std::ifstream(vertex_path);
    std::ifstream fragment_stream = std::ifstream(fragment_path);
    if (!vertex_stream.is_open()) { throw std::runtime_error("vertex_path not valid path"); }
    if (!fragment_stream.is_open()) { throw std::runtime_error("fragment_path not valid path"); }

    std::stringstream vertex_ss, fragment_ss;
    vertex_ss << vertex_stream.rdbuf();
    fragment_ss << fragment_stream.rdbuf();

    std::string vertex_source = vertex_ss.str();
    std::string fragment_source = fragment_ss.str();
    const char* vsrc = vertex_source.c_str();
    const char* fsrc = fragment_source.c_str();

    int vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_ID, 1, &vsrc, NULL);
    glCompileShader(vertex_shader_ID);

    int fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_ID, 1, &fsrc, NULL);
    glCompileShader(fragment_shader_ID);

    program_ID = glCreateProgram();
    glAttachShader(program_ID, vertex_shader_ID);
    glAttachShader(program_ID, fragment_shader_ID);
    glLinkProgram(program_ID);

    glDeleteShader(vertex_shader_ID);
    glDeleteShader(fragment_shader_ID);
}

void Shader::use(){
    glUseProgram(program_ID);
}

void Shader::setFloat(const std::string& attribute, float value){
    int loc = glGetUniformLocation(program_ID, attribute.c_str());
    glUniform1f(loc, value);
}

void Shader::setFloatVec(const std::string& attribute, int size, float values[]){
    if (size != 2 && size != 3 && size != 4){
        throw std::runtime_error("This size vector is not permitted in shader");
    }

    int loc = glGetUniformLocation(program_ID, attribute.c_str());
    if (size == 2){
        glUniform2f(loc, values[0], values[1]);
    } else if (size == 3){
        glUniform3f(loc, values[0], values[1], values[2]);
    } else {
        glUniform4f(loc, values[0], values[1], values[2], values[3]);
    }
}

