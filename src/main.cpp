#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

std::string read_file(const std::string& path){
    std::ifstream file(path);
    if (!file.is_open()){
        throw std::runtime_error("file does not exist");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string vert_src = read_file("shaders/vertShader.vert");
const char* vertexShaderSource = vert_src.c_str();

std::string frag1_src = read_file("shaders/fragShader1.frag");
const char* fragShaderSource1 = frag1_src.c_str();

std::string frag2_src = read_file("shaders/fragShader2.frag");
const char* fragShaderSource2 = frag2_src.c_str();

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window;

    window = glfwCreateWindow(800, 600, "Physics Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();        
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Shaders and programs
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Shader vertex compilation failed" << infoLog << "\n";
    }

    unsigned int fragShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader1, 1, &fragShaderSource1, NULL);
    glCompileShader(fragShader1);

    unsigned int fragShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader2, 1, &fragShaderSource2, NULL);
    glCompileShader(fragShader2);

    unsigned int shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragShader1);
    glLinkProgram(shaderProgram1);

    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragShader2);
    glLinkProgram(shaderProgram2);


    glDeleteShader(vertexShader);
    glDeleteShader(fragShader1);
    glDeleteShader(fragShader2);

    glGetProgramiv(shaderProgram2, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "Shader Program compilated failed" << infoLog << "\n";
    }

    //VAO, VBO, EBO, drawing
    float vertices1[] = {
        -0.7f, -0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.3f, -0.7f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    float vertices2[] = {
        0.5f, 0.5f, 0.0f,  // top corner 2
        0.7f, 0.2f, 0.0f,    // right corner 2
        0.3f, 0.2f, 0.0f,
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









    while(!glfwWindowShouldClose(window)){
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);



        glUseProgram(shaderProgram2);
        float time = glfwGetTime();
        float rgba_val = (0.5 * sin(time)) + 0.5;
        int ourColorUniformLocation = glGetUniformLocation(shaderProgram2, "ourColor");
        glUniform4f(ourColorUniformLocation, 0.0f, rgba_val, 0.0f, 1.0f);


        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
        

    glfwTerminate();
    return 0;
}