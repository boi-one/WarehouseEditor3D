#version 330 core
out vec4 FragColor;

in vec3 color;
uniform vec3 mColor;

void main()
{
    FragColor = vec4(mColor, 1.0f);
} 