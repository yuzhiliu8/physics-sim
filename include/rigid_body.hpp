#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include "vector"
#include "glm/glm.hpp"
#include "utils.hpp"
#include <memory>
#include "shader.hpp"

//abstract class
class RigidBody {
    public:
        virtual void render(std::shared_ptr<Shader> shader) = 0;
        void update_pos();
        void set_pos(const glm::vec3& pos);
        void set_velocity(const glm::vec3& velocity);
        void set_acceleration(const glm::vec3& acceleration);
        const glm::vec3& pos();
        const glm::vec3 velocity();
        const glm::vec3& acceleration();
        virtual ~RigidBody() = default;
    
    protected:
        virtual void draw_static_vertices() = 0;
        std::vector<float> vertices_;
        glm::vec3 pos_;
        glm::vec3 last_pos_;
        glm::vec3 acceleration_;

        unsigned int VBO;
        unsigned int VAO;
};
#endif