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
#include "camera.hpp"

class PhysicsSim
{
    public:
        PhysicsSim(int width, int height);
        void initialize();
        void set_shader(std::shared_ptr<Shader> shader);
        void add_obj(std::shared_ptr<Circle> c);
        void start();
        ~PhysicsSim();

    private:
        GLFWwindow* window_;
        int width_;
        int height_;
        std::vector<std::shared_ptr<Circle>> objs_;
        std::shared_ptr<Shader> shader_;

        Camera camera_;
        const float CAM_SPEED = 0.5;
        float last_mouse_x;
        float last_mouse_y;
        bool first_mouse = true;

        void render();
        void update_physics();
        void process_input(float frame_time);
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};

#endif