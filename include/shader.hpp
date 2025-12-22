#ifndef SHADER_HPP
#define SHADER_HPP

#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



class Shader{
    public:
        Shader(const std::string& vertex_path, const std::string& fragment_path);
        void use();
        void setFloat(const std::string& attribute, float value);
        void setFloatVec(const std::string& attribute, int size, float values[]);
        void setMat4(const std::string& attribute, const glm::mat4& mat);

        ~Shader();
        
        

    
    private:
        unsigned int program_id_;
        



};

#endif