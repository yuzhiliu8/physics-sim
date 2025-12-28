#include "rigid_body.hpp"


void RigidBody::update_pos(){
    float dt2 = phys::dt2;
    glm::vec3 new_pos = 2.0f*pos_ - last_pos_ + acceleration_*dt2;
    last_pos_ = pos_;
    pos_ = new_pos;
}

void RigidBody::set_pos(const glm::vec3& pos){
    glm::vec3 preserved = velocity();
    pos_ = pos;
    set_velocity(preserved);

}
void RigidBody::set_velocity(const glm::vec3& vel){
    last_pos_ = pos_ - (float)phys::dt*vel;
}

void RigidBody::set_acceleration(const glm::vec3& acceleration){
    acceleration_ = acceleration;
}
const glm::vec3& RigidBody::pos(){
    return pos_;
}
const glm::vec3 RigidBody::velocity(){
    return glm::vec3((pos_ - last_pos_) / (float)phys::dt);
}

const glm::vec3& RigidBody::acceleration(){
    return acceleration_;
}