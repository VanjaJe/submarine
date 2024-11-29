#version 330 core
out vec4 FragColor;

uniform vec3 color; // Uniformna promenljiva za boju

void main()
{
    FragColor = vec4(color, 1.0f); // Postavi boju na crvenu (1.0, 0.0, 0.0)
