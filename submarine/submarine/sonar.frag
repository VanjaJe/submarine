#version 330 core
out vec4 FragColor;

uniform float time; // Vreme za pulsiranje

void main() {
    float intensity = 0.2 + 0.8 * (0.5 + 0.5 * sin(time));
    FragColor = vec4(0.0, intensity, 0.0, 1.0); // Zeleno svetlo
}