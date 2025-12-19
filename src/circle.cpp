#include "circle.hpp"

const int Circle::RESOLUTION = 100;

Circle::Circle(Shader &shader, float x, float y, float z, float radius){
    shader_ = std::make_shared<Shader>(shader);
    center_.x = x;
    center_.y = y;
    center_.z = z;
    radius_ = radius;
    int size = 9 * RESOLUTION;
    vertices_.reserve(size); //should always stay at this capacity

    //VAO, VBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size* sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glBindVertexArray(0); //unbind current, which is our VAO
}

void Circle::draw_vertices(){
    vertices_.clear();
    double pi = std::numbers::pi;
    float angle_incr = 2 * pi / RESOLUTION;
    /*
    x = r cos theta
    y = r sin theta 
    */

    for (int i = 0; i < RESOLUTION; i++){
        float theta = i * angle_incr;
        //on the radius of the circle
        float x1 = center_.x + (radius_*cos(theta));
        float y1 = center_.y + (radius_ * sin(theta));
        float z1 = 0.0;
        //x only
        float x2 = center_.x + (radius_*cos(theta + angle_incr));
        float y2 = center_.y + (radius_ * sin(theta + angle_incr));
        float z2 = 0.0;

        vertices_.push_back(x1);
        vertices_.push_back(y1);
        vertices_.push_back(z1);
        vertices_.push_back(x2);
        vertices_.push_back(y2);
        vertices_.push_back(z2);
        vertices_.push_back(center_.x);
        vertices_.push_back(center_.y);
        vertices_.push_back(center_.z);
    }
}

void Circle::render(){
    shader_->use();
    draw_vertices();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_.size() * sizeof(vertices_[0]), vertices_.data());
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size() / 3);
}



