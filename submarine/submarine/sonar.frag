#version 330 core
out vec4 FragColor;

uniform float time; // Vreme za pulsiranje

void main() {
    float intensity = 0.3 + 0.3 * (0.5 + 0.5 * sin(4.0 * time));
    FragColor = vec4(0.0, intensity, 0.0, 1.0); // Zeleno svetlo
}