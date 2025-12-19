#ifndef PHYSICS_SIM_HPP
#define PHYSICS_SIM_HPP

#include <string>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cmath>
#include "shader.hpp"
#include "circle.hpp"



class PhysicsSim
{
    public:
        PhysicsSim(int width, int height);
        void initialize();
        void start();


    private:
        GLFWwindow* window_;
        int width_;
        int height_;
        


};

#endif