#ifndef PHYSICS_SIM_HPP
#define PHYSICS_SIM_HPP

#include <string>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cmath>
#include "shader.hpp"
#include "circle.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class PhysicsSim
{
    public:
        PhysicsSim(int width, int height);
        void initialize();
        void set_shader(std::shared_ptr<Shader> shader);
        void add_obj(std::shared_ptr<Circle> c);
        void start();

    private:
        GLFWwindow* window_;
        int width_;
        int height_;
        std::vector<std::shared_ptr<Circle>> objs_;
        std::shared_ptr<Shader> shader_;

        void render();
        void update_physics();
        


};

#endif