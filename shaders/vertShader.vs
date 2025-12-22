#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;


out vec4 vertexColor;
uniform mat4 transform;


out vec3 vertexPos;
void main()
{
   gl_Position = transform * vec4(aPos, 1.0);
   vertexColor = vec4(aColor.xyz, 1.0);
   vertexPos = aPos;
}