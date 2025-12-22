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

void PhysicsSim::start(){
    //VAO, VBO, EBO, drawing
    float vertices1[] = {
        -0.7f, -0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.3f, -0.7f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    float vertices2[] = {
        0.5f, 0.5f, 0.0f,  // top corner 2
        0.7f, 0.2f, 0.0f,    // right corner 2
        -0.3f, -0.2f, 0.0f,
    };

    // For EBO drawing
    // unsigned int rect_indices[] = {
    //     0, 1, 2,
    //     1, 2, 3
    // };
    // TRIANGLE 1
    unsigned int VAO1;
    
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    unsigned int VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); //unbind VAO1

    //TRIANGLE 2
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    // for element index based drawing
    // unsigned int EBO;
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);
    // Circle c1(shader2, 0.5, 0.5);
    // Circle c2(shader2, 0.0, 0.0, 0.0, 0.1);


    int i = 0;
    double last_time = glfwGetTime();
    double accumulator = 0.0;
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
        c->render();
        // std::cout << c->get_velocity().y << "\n";
    }
}

void PhysicsSim::update_physics(){
    for (std::shared_ptr<Circle> &c : objs_){
        c->update_pos();
        glm::vec3 pos = c->get_pos();
        glm::vec3 v = c->get_velocity();
        if (pos.y - c->radius() <= -1){
            pos.y = -1.0 + c->radius();
            c->set_pos(pos.x, pos.y, pos.z);
            c->set_velocity(v.x, -v.y, v.z);
        }
        std::cout << "X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z << "\n";
    }
}