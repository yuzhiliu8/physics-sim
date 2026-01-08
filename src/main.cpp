#include "physics_sim.hpp"
#include "circle.hpp"
#include "shader.hpp"
#include "utils.hpp"
#include "sphere.hpp"

int main(){

    PhysicsSim sim(1280, 720, glm::vec3(0.0f, 10.0f, 50.0f));
    sim.initialize();

    sim.set_bounding_box(25.0f, -25.0f, 25.0f, -25.0f, 50.0f, 0.0f);
    std::shared_ptr<Shader> shader = std::make_shared<Shader>("shaders/vertShader.vs", "shaders/fragShader2.fs");
    // shader->setFloat("alpha", 1.0);
    // std::cout << "PhysicsSim addr: " << &sim << std::endl;
    // std::cout << "shader_ addr: " << &(shader) << std::endl;
    sim.set_shader(shader);

    std::shared_ptr<RigidBody> s1 = std::make_shared<Sphere>(glm::vec3(0.0f, 1.85f, 0.0f), 0.1f);
    s1->set_velocity(glm::vec3(5.0f, 0.0f, 0.0f));
    s1->set_acceleration(glm::vec3(0.0, -phys::GRAVITY, 0.0));
    std::shared_ptr<RigidBody> s2 = std::make_shared<Sphere>(glm::vec3(-10.0f, 1.0f, -3.0f), 0.5f);
    s2->set_velocity(glm::vec3(100.0, 100.0, 1000.0));
    s2->set_acceleration(glm::vec3(0.0, -phys::GRAVITY, 0.0));

    sim.add_obj(s1);
    sim.add_obj(s2);


    sim.start();
    
    // std::vector<unsigned int> vec;
    // vec.reserve(1);
    // add_line(vec, 1, 2);
    return 0;
}

