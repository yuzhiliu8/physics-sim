#include "physics_sim.hpp"

PhysicsSim::PhysicsSim(int width, int height)
    : width_(width), 
    height_(height),
    camera_(glm::vec3(0.0, 0.0, 3.0), (float)width_ / (float)height_)
{}

void PhysicsSim::initialize(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
    last_mouse_x = width_/2;
    last_mouse_y = height_/2;
}

void PhysicsSim::set_shader(std::shared_ptr<Shader> shader){
    shader_ = shader;
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
        glClear(GL_COLOR_BUFFER_BIT);
        
        double now = glfwGetTime();
        double frame_time = now - last_time;
        last_time = now;
        process_input(frame_time);

        accumulator += frame_time;
        while(accumulator >= phys::dt){
            update_physics();
            accumulator -= phys::dt;
        }
        shader_->setMat4("view", camera_.view());
        shader_->setMat4("projection", camera_.projection());
        render();
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
    glfwTerminate();
}

void PhysicsSim::add_obj(std::shared_ptr<Circle> c){
    objs_.push_back(c); 
}

void PhysicsSim::render(){
    for (std::shared_ptr<Circle> &c : objs_){
        c->render(shader_);
        std::cout << c->velocity().y << "\n";
    }
}

void PhysicsSim::update_physics(){
    for (std::shared_ptr<Circle> &c : objs_){
        c->update_pos();
        glm::vec3 pos = c->pos();
        glm::vec3 v = c->velocity();
        if (pos.y - c->radius() <= -1){
            pos.y = -1.0 + c->radius();
            c->set_pos(pos.x, pos.y, pos.z);
            c->set_velocity(v.x, -v.y, v.z);
        }
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