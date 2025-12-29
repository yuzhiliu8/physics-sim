#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>

namespace phys{
    inline constexpr double dt = 1.0 / 60.0;
    inline constexpr double dt2 = dt * dt;
    inline constexpr double GRAVITY = 9.81;
}

namespace color {
    inline float green[] = {0.25f, 0.61f, 0.04f, 1.0f};
    inline float blue[] = {0.5f, 0.5f, 1.0f, 1.0f};
}

inline void add_triple(std::vector<float>& vec, float x, float y, float z){
    vec.push_back(x);
    vec.push_back(y);
    vec.push_back(z);
}

inline void add_pair(std::vector<unsigned int>& vec, unsigned int x, unsigned int y){
    vec.push_back(x);
    vec.push_back(y);
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