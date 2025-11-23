#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec3 offset;

out vec4 vertexColor;
out vec3 vertexPos;
void main()
{
   gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z + offset.z, 1.0);
   vertexColor = vec4(aColor.xyz, 1.0);
   vertexPos = aPos;
}