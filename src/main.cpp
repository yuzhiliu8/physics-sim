#include "physics_sim.hpp"
#include "circle.hpp"
#include "shader.hpp"
#include "utils.hpp"




int main(){
    PhysicsSim sim(600, 600);
    sim.initialize();

    Shader shader1("shaders/vertShader.vs", "shaders/fragShader1.fs");
    // Shader shader2("shaders/vertShader.vs", "shaders/fragShader2.fs");
    float blue[] = {0.5, 0.0, 1.0, 1.0};
    std::shared_ptr<Shader> shader2 = std::make_shared<Shader>("shaders/vertShader.vs", "shaders/fragShader2.fs");

    // circles.emplace_back(shader2, 0.5, 0.5);
    // circles.emplace_back(shader2, 0.0, 0.0, 0.0, 0.1);
    std::shared_ptr<Circle> c = std::make_shared<Circle>(shader2, 0.5, 0.5);
    shader2->setFloatVec("ourColor", 4, blue);
    c->set_velocity(-0.1, 0.0, 0.0);
    c->set_acceleration(0.0, -phys::GRAVITY, 0.0);
    sim.add_obj(c);

    sim.start();
    

    return 0;
}
