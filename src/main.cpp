#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "physics_sim.hpp"



int main(){
    PhysicsSim sim(600, 600);
    sim.initialize();
    sim.start();

    return 0;
}
