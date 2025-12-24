#include "circle.hpp"

const int Circle::RESOLUTION = 100;

Circle::Circle(float x, float y, float z, float radius){
    center_.pos = glm::vec3(x, y, z);
    center_.last_pos = glm::vec3(x, y, z);
    radius_ = radius;
    int size = 9 * RESOLUTION;
    vertices_.reserve(size); //should always stay at this capacity
    draw_static_vertices();

    //VAO, VBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glBindVertexArray(0); //unbind current, which is our VAO
}

void Circle::draw_static_vertices(){ //should only be called once!
    // vertices_.clear();
    //Do it WRT origin
    double pi = std::numbers::pi;
    float angle_incr = 2*pi / RESOLUTION;
    glm::vec3 origin;
    /*
    x = r cos theta
    y = r sin theta
    */
    for (int i = 0; i < RESOLUTION; i++){
        float theta = i * angle_incr;
        //on the radius of the circle
        float x1 = radius_*cos(theta);
        float y1 = radius_*sin(theta);
        float z1 = 0.0;

        float x2 = radius_*cos(theta + angle_incr);
        float y2 = radius_*sin(theta + angle_incr);
        float z2 = 0.0;

        vertices_.push_back(x1);
        vertices_.push_back(y1);
        vertices_.push_back(z1);
        vertices_.push_back(x2);
        vertices_.push_back(y2);
        vertices_.push_back(z2);
        vertices_.push_back(0.0); //origin
        vertices_.push_back(0.0);
        vertices_.push_back(0.0);
    }
}

void Circle::update_pos(){
    float dt2 = phys::dt2;
    glm::vec3 new_pos = 2.0f*center_.pos - center_.last_pos + center_.acceleration*dt2;
    center_.last_pos = center_.pos;
    center_.pos = new_pos;
}

void Circle::render(std::shared_ptr<Shader> shader){
    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, center_.pos);
    shader->setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size() / 3);
}

void Circle::set_pos(float x, float y, float z){
    glm::vec3 preserved = velocity();
    center_.pos.x = x;
    center_.pos.y = y;
    center_.pos.z = z;

    set_velocity(preserved.x, preserved.y, preserved.z);
}
/*
(curr - last) / t = v
curr - last = vt
last = curr - vt
*/
void Circle::set_velocity(float x, float y, float z){
    center_.last_pos.x = center_.pos.x - x*phys::dt;
    center_.last_pos.y = center_.pos.y - y*phys::dt;
    center_.last_pos.z = center_.pos.z - z*phys::dt;
}

void Circle::set_acceleration(float x, float y, float z){
    center_.acceleration = glm::vec3(x, y, z);
}

float Circle::radius(){
    return radius_;
}

glm::vec3 Circle::pos(){
    return center_.pos;
}

glm::vec3 Circle::velocity(){
    return glm::vec3(
        (center_.pos.x - center_.last_pos.x) / phys::dt,
        (center_.pos.y - center_.last_pos.y) / phys::dt,
        (center_.pos.z - center_.last_pos.z) / phys::dt
    );
}
glm::vec3 Circle::acceleration(){ return center_.acceleration; }

Circle::~Circle(){
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}




