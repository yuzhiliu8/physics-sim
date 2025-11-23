#ifndef SHADER_HPP
#define SHADER_HPP

#include "glad/glad.h"
#include<string>
#include<fstream>
#include<sstream>



class Shader{
    public:
        Shader(const std::string& vertex_path, const std::string& fragment_path);
        void use();
        void setFloat(const std::string& attribute, float value);
        void setFloatVec(const std::string& attribute, int size, float values[]);
        
        

    
    private:
        unsigned int program_ID;
        



};

#endif