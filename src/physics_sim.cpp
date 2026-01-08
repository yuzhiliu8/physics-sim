#include "physics_sim.hpp"

PhysicsSim::PhysicsSim(int width, int height)
    : width_(width), 
    height_(height),
    camera_(glm::vec3(0.0, 0.0, 3.0), (float)width_ / (float)height_)
{}

PhysicsSim::PhysicsSim(int width, int height, glm::vec3 cam_pos)
    : width_(width), 
    height_(height),
    camera_(cam_pos, (float)width_ / (float)height_)
{}

void PhysicsSim::initialize(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);  // 24-bit depth buffer

    window_ = glfwCreateWindow(width_, height_, "Physics Engine", NULL, NULL);
    if (!window_)
    {
        glfwTerminate();        
        throw std::runtime_error("Could not create window");
    }

    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width_, height_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window_, this);
    glfwSetCursorPosCallback(window_, mouse_callback);  
    glEnable(GL_DEPTH_TEST);

    last_mouse_x = width_/2;
    last_mouse_y = height_/2;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // glBindVertexArray(VAO);

}



void PhysicsSim::set_shader(std::shared_ptr<Shader> shader){
    shader_ = shader;
}

/* 8 vertices
0,4      1,5
    (0,0)
2,6      3,7

*/
void PhysicsSim::set_bounding_box(
    float front, float back,  //pos Z, neg Z
    float right, float left, 
    float top, float bottom){

    if (front <= back || right <= left || top <= bottom){
        throw std::runtime_error("Bounding Box coordinate errors");
    }

    vertices_.clear();
    vertices_.reserve(8 * 3);
    
    
    add_triple(vertices_, left, bottom, back);  //0
    add_triple(vertices_, right, bottom, back); //1
    add_triple(vertices_, left, bottom, front); //2
    add_triple(vertices_, right, bottom, front);//3
    add_triple(vertices_, left, top, back);     //4
    add_triple(vertices_, right, top, back);    //5
    add_triple(vertices_, left, top, front);    //6
    add_triple(vertices_, right, top, front);   //7


    indices_.clear();
    indices_.reserve(24);
    indices_.push_back(0); //bottom face
    indices_.push_back(1);
    indices_.push_back(2);
    indices_.push_back(1);
    indices_.push_back(2);
    indices_.push_back(3);
    
    add_pair(indices_, 0, 4); //connecting edges
    add_pair(indices_, 1, 5);
    add_pair(indices_, 2, 6);
    add_pair(indices_, 3, 7);
    add_pair(indices_, 4, 5); //top face
    add_pair(indices_, 4, 6);
    add_pair(indices_, 6, 7);
    add_pair(indices_, 5, 7);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);  //unbind
    glBindVertexArray(0);

    has_bounding_box_ = true;
    bounding_box_.front = front;
    bounding_box_.back = back;
    bounding_box_.right = right;
    bounding_box_.left = left;
    bounding_box_.top = top;
    bounding_box_.bottom = bottom;
}

void PhysicsSim::start(){
    if (!shader_){
        throw std::runtime_error("No shader set for Physics Sim");
    }

    int i = 0;
    double last_time = glfwGetTime();
    double accumulator = 0.0;

    while(!glfwWindowShouldClose(window_)){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        double now = glfwGetTime();
        double frame_time = now - last_time;
        last_time = now;
        process_input(frame_time);

        shader_->setMat4("view", camera_.view());
        shader_->setMat4("projection", camera_.projection());
        if (sim_started_){
            accumulator += frame_time;
            time_elapsed_ += frame_time;
            while(accumulator >= phys::dt){
                update_physics();
                accumulator -= phys::dt;
            }
            std::cout << "TIME ELAPSED: " << time_elapsed_ << "\n";
        }
        render();
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void PhysicsSim::add_obj(std::shared_ptr<RigidBody> c){
    objs_.push_back(c); 
}

void PhysicsSim::render(){
    if (has_bounding_box_){ //render bounding box
        shader_->setMat4("model", glm::mat4(1.0f));
        shader_->setFloatVec("ourColor", 4, color::green);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawElements(GL_LINES, indices_.size()-6 , GL_UNSIGNED_INT, (void*) (6 * sizeof(unsigned int)));
    }
    for (std::shared_ptr<RigidBody> &c : objs_){
        c->render(shader_);
        // std::cout << c->velocity().y << "\n";
    }
}

void PhysicsSim::update_physics(){
    for (std::shared_ptr<RigidBody> &b : objs_){
        b->update_pos();
        glm::vec3 pos = b->pos(); //copy to pos, can modify pos
        glm::vec3 v = b->velocity();

        if (has_bounding_box_){ //bounding box collision checks, assumes obj is smaller than bounding box
            float rad{ b->collision_radius() };
            bool collided{ false };
            if (pos.y - rad <= bounding_box_.bottom){
                collided = true;
                pos.y = bounding_box_.bottom + rad;
                v.y = -phys::restitution * v.y;
            } else if (pos.y + rad >= bounding_box_.top){
                collided = true;
                pos.y = bounding_box_.top - rad;
                v.y = -phys::restitution * v.y;
            }

            if (pos.x + rad >= bounding_box_.right){
                collided = true;
                pos.x = bounding_box_.right - rad;
                v.x = -phys::restitution * v.x;
            } else if (pos.x - rad <= bounding_box_.left){
                collided = true;
                pos.x = bounding_box_.left + rad;
                v.x = -phys::restitution * v.x;
            }

            if (pos.z - rad <= bounding_box_.back){
                collided = true;
                pos.z = bounding_box_.back + rad;
                v.z = -phys::restitution * v.z;
            } else if (pos.z + rad >= bounding_box_.front){
                collided = true;
                pos.z = bounding_box_.front - rad;
                v.z = -phys::restitution * v.z;
            }

            if (collided){
                b->set_pos(pos);
                b->set_velocity(v);
            }
        }
        // if (pos.y - b->collision_radius() <= 0.0f){
        //     pos.y = b->collision_radius();
        //     b->set_pos(glm::vec3(pos.x, pos.y, pos.z));
        //     b->set_velocity(glm::vec3(v.x, -0.75f * v.y, v.z));
        // }
        std::cout << "X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z << "\n";
    }
}

void PhysicsSim::process_input(float frame_time){
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(window_, true);
    }
    if(glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS){
        camera_.forward(CAM_SPEED * frame_time);
    }
    if(glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS){
        camera_.backward(CAM_SPEED * frame_time);
    }
    if(glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS){
        camera_.left(CAM_SPEED * frame_time);
    }
    if(glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS){
        camera_.right(CAM_SPEED * frame_time);
    }
    if(glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera_.up(CAM_SPEED * frame_time);
    }
    if(glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera_.down(CAM_SPEED * frame_time);
    }
    if(glfwGetKey(window_, GLFW_KEY_ENTER) == GLFW_PRESS && !sim_started_){
        sim_started_ = !sim_started_;
    }
}

void PhysicsSim::framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    PhysicsSim* app = static_cast<PhysicsSim*>(glfwGetWindowUserPointer(window));
    app->width_ = width;
    app->height_ = height;
    app->camera_.set_aspect_ratio((float)app->width_/(float)app->height_);
}

void PhysicsSim::mouse_callback(GLFWwindow* window, double xpos, double ypos){
    PhysicsSim* app = static_cast<PhysicsSim*>(glfwGetWindowUserPointer(window));
    if (app->first_mouse)
        {
            app->last_mouse_x = xpos;
            app->last_mouse_y = ypos;
            app->first_mouse = false;
        }
    float x_offset = xpos - app->last_mouse_x;
    float y_offset = ypos - app->last_mouse_y;
    app->last_mouse_x = xpos;
    app->last_mouse_y = ypos;
    app->camera_.acc_yaw(x_offset);
    app->camera_.acc_pitch(-y_offset);
}

PhysicsSim::~PhysicsSim(){
    std::cout << "Terminating Physics Sim" << "\n";
    objs_.clear();
    shader_ = nullptr;
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (EBO != 0) glDeleteBuffers(1, &EBO);
    if(window_){
        glfwSetWindowUserPointer(window_, nullptr);
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}