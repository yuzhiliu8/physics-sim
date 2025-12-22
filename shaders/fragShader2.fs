#version 330 core
out vec4 FragColor;
uniform vec4 ourColor;

in vec3 vertexPos;
void main()
{
    // FragColor = vec4(vertexPos.xyz, 1.0);
    FragColor = ourColor;
}