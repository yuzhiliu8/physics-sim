#include "physics_sim.hpp"
#include "circle.hpp"
#include "shader.hpp"
#include "utils.hpp"

int main(){

    float blue[] = {0.5, 0.5, 1.0, 1.0};
    PhysicsSim sim(1280, 720);
    sim.initialize();

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("shaders/vertShader.vs", "shaders/fragShader2.fs");
    sim.set_shader(shader);

    // circles.emplace_back(shader2, 0.5, 0.5);
    // circles.emplace_back(shader2, 0.0, 0.0, 0.0, 0.1);
    std::shared_ptr<Circle> c = std::make_shared<Circle>(0.5, 0.5, 0.0, 0.5);
    shader->setFloatVec("ourColor", 4, blue);
    c->set_velocity(0.0, 0.0, 0.0);
    c->set_acceleration(0.0, -phys::GRAVITY, 0.0);
    sim.add_obj(c);

    sim.start();
    

    return 0;
}

