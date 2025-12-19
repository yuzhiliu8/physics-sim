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
}

void PhysicsSim::start(){
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
    Shader shader1("shaders/vertShader.vs", "shaders/fragShader1.fs");
    Shader shader2("shaders/vertShader.vs", "shaders/fragShader2.fs");

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

    std::vector<Circle> circles;
    circles.emplace_back(shader2, 0.5, 0.5);
    circles.emplace_back(shader2, 0.0, 0.0, 0.0, 0.1);


    int i = 0;
    while(!glfwWindowShouldClose(window_)){
        process_input(window_);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glUseProgram(shaderProgram1);
        // shader1.use();
        // glBindVertexArray(VAO1);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // shader2.use();
        // float time = glfwGetTime();
        // float green = (0.5 * sin(time)) + 0.5;
        // float rgb_values[] = {0.0f, green, 0.0f, 1.0f};
        // shader2.setFloatVec("ourColor", 4, rgb_values);

        // float offset[] = {0.01f * i, 0.01f * i, 0.1f};
        // i++;
        // shader2.setFloatVec("offset", 3, offset);


        // glBindVertexArray(VAO2);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        for (size_t i = 0; i < circles.size(); i++){
            circles[i].render();
        }


        glfwSwapBuffers(window_);
        glfwPollEvents();

    }
        
    glfwTerminate();

}




