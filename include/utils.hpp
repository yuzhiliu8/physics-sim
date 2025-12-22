#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>

namespace phys{
    inline constexpr double dt = 1.0 / 60.0;
    inline constexpr double dt2 = dt * dt;
    inline constexpr double GRAVITY = 9.81;
}

struct Point {
    glm::vec3 pos;
    glm::vec3 last_pos;
    glm::vec3 acceleration;

    Point(){ //at rest
        pos = glm::vec3();
        last_pos = glm::vec3();
        acceleration = glm::vec3();
    }

    Point(float x, float y, float z){ //at rest
        pos = glm::vec3(x, y, z);
        last_pos = glm::vec3(x, y, z);
        acceleration = glm::vec3();
    }
};


#endif