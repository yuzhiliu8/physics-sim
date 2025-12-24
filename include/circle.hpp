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
        Circle(float x, float y, float z=0.0, float radius=0.2);
        void update_pos();
        void render(std::shared_ptr<Shader> shader);
        void set_pos(float x, float y, float z);
        void set_velocity(float x, float y, float z);
        void set_acceleration(float x, float y, float z);
        float radius();
        glm::vec3 pos();
        glm::vec3 velocity();
        glm::vec3 acceleration();
        ~Circle();

    private:
        static const int RESOLUTION;
        Point center_;
        float radius_;
        std::vector<float> vertices_;
        unsigned int VBO;
        unsigned int VAO;

        void draw_static_vertices();
};

#endif