#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform mat4 model;
uniform mat4 transform; //transform = projection * view
//projection is the camera and view the position 
void main()
{
    gl_Position = transform * model * vec4(aPos, 1.0);
    color = aColor;
}