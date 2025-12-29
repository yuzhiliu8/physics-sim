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
#include "rigid_body.hpp"

class PhysicsSim
{
    public:
        PhysicsSim(int width, int height);
        PhysicsSim(int width, int height, glm::vec3 cam_pos);
        void initialize();
        void set_shader(std::shared_ptr<Shader> shader);
        void add_obj(std::shared_ptr<RigidBody> c);
        void start();

        /*
        Creates a bounding box w/ a square base
        X: [-base/2, base/2]
        Y: [0, height]
        Z: [-base/2, base/2]
        */ 
        void set_bounding_box(float width, float height);
        ~PhysicsSim();

    private:
        GLFWwindow* window_;
        int width_;
        int height_;
        std::vector<std::shared_ptr<RigidBody>> objs_;
        std::shared_ptr<Shader> shader_;
        std::vector<float> vertices_;
        std::vector<unsigned int> indices_;
        bool bounding_box_ {false};
        bool sim_started_ {false};
        float time_elapsed_ {};

        Camera camera_;
        const float CAM_SPEED = 10.0f;
        float last_mouse_x;
        float last_mouse_y;
        bool first_mouse = true;

        void render();
        void update_physics();
        void process_input(float frame_time);
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

        unsigned int VAO{};
        unsigned int VBO{};
        unsigned int EBO{};
};

#endif