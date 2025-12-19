#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shader.hpp"
#include <memory>
#include <vector>
#include <cmath>
#include <numbers>

struct Point
{
    float x;
    float y;
    float z;
};


class Circle{
    public:
        Circle(Shader &shader, float x, float y, float z=0.0, float radius=0.2);
        void render();


    private:
        static const int RESOLUTION;
        std::shared_ptr<Shader> shader_;
        Point center_;
        float radius_;
        std::vector<float> vertices_;
        unsigned int VAO;
        unsigned int VBO;

        void draw_vertices();

        



};

#endif