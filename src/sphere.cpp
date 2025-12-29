#include "sphere.hpp"

const int Sphere::RESOLUTION = 100;

Sphere::Sphere(const glm::vec3& p, float radius){
    pos_ = p;
    last_pos_ = p;
    radius_ = radius;
    draw_static_vertices();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_indices_.size() * sizeof(unsigned int), element_indices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    //unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
/*
x = r sin phi cos theta
y = r sin phi sin theta
z = r cos phi 

[(p0 t0), (p0 t1), (p0 t2) ... (p0 tn)]  RESOLUTION = n
[(p1 t0)  ...                  (p1 tn)]
...
[(p{n+1}, t0)] ...             (p{n+1} tn)]

[i0 i1]
[i2 i3]
*/
void Sphere::draw_static_vertices(){
    double dtheta {2 * std::numbers::pi / RESOLUTION};
    double dphi {std::numbers::pi / RESOLUTION};

    for (int i = 0; i <= RESOLUTION; i++){
        double phi {i * dphi};
        for (int j = 0; j < RESOLUTION; j++){
            double theta {j * dtheta};
            vertices_.push_back(radius_ * sin(phi) * cos(theta)); //x
            vertices_.push_back(radius_ * sin(phi) * sin(theta)); //y
            vertices_.push_back(radius_ * cos(phi)); //z
        }
    }
    for (int i = 0; i <= RESOLUTION; i++){
        for (int j = 0; j < RESOLUTION; j++){
            int i0 = i * RESOLUTION + j;
            int i1 = i * RESOLUTION + (j+1) % RESOLUTION;
            int i2 = (i+1) * RESOLUTION + j;
            int i3 = (i+1) * RESOLUTION + (j+1) % RESOLUTION;

            element_indices_.push_back(i0); //Triangle 1
            element_indices_.push_back(i2);
            element_indices_.push_back(i1);
            element_indices_.push_back(i1); //Triangle 2
            element_indices_.push_back(i2);
            element_indices_.push_back(i3);
        }
    }
}

float Sphere::collision_radius(){
    return radius_;
}

void Sphere::render(std::shared_ptr<Shader> shader) {
    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos_);
    shader->setMat4("model", model);
    shader->setFloatVec("ourColor", 4, color::blue);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, element_indices_.size(), GL_UNSIGNED_INT, 0);
}

Sphere::~Sphere(){
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}