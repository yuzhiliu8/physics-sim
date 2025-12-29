#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "rigid_body.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numbers>
#include <iostream>

class Sphere : public RigidBody{
    public:
        Sphere(const glm::vec3& p, float radius);
        void render(std::shared_ptr<Shader> shader) override;
        float collision_radius() override;
        ~Sphere();
    
    private:
        static const int RESOLUTION;
        float radius_;
        void draw_static_vertices() override;
        std::vector<unsigned int> element_indices_;
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;



};
#endif