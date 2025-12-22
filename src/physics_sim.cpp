#include "physics_sim.hpp"

PhysicsSim::PhysicsSim(int width, int height){
    width_ = width;
    height_ = height;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

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

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)width_ / (float)height_, 0.1f, 100.0f);
    shader_->setMat4("view", view);
    shader_->setMat4("projection", projection);

    while(!glfwWindowShouldClose(window_)){
        process_input(window_);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        double now = glfwGetTime();
        double frame_time = now - last_time;
        last_time = now;

        accumulator += frame_time;
        while(accumulator >= phys::dt){
            update_physics();
            accumulator -= phys::dt;
        }
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