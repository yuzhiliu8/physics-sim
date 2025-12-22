#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shader.hpp"
#include <memory>
#include <vector>
#include <cmath>
#include <numbers>
#include "utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Circle{
    public:
        Circle(std::shared_ptr<Shader> shader, float x, float y, float z=0.0, float radius=0.2);
        void update_pos();
        void render();
        void set_pos(float x, float y, float z);
        void set_velocity(float x, float y, float z);
        void set_acceleration(float x, float y, float z);
        float radius();
        glm::vec3 get_pos();
        glm::vec3 get_velocity();
        glm::vec3 get_acceleration();


    private:
        static const int RESOLUTION;
        std::shared_ptr<Shader> shader_;
        Point center_;
        float radius_;
        std::vector<float> vertices_;
        unsigned int VAO;
        unsigned int VBO;

        void draw_static_vertices();
};

#endif