#version 330 core

uniform vec4 color; // Vreme za pulsiranje

out vec4 FragColor;

void main()
{
    FragColor = vec4(color);
}