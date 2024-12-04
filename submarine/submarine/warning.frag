#version 330 core

in vec2 chTex; // Koordinate teksture
out vec4 outCol;

uniform sampler2D uTex; // Teksturna jedinica
uniform float time; // Uniform za vreme (proslje?uje se iz aplikacije)

void main()
{
    // Uzimamo boje sa teksture
    vec4 texColor = texture(uTex, chTex);

    float pulse = sin(time * 3.0); // Množimo sa 3 da bi treperenje bilo brže
    pulse = (pulse + 1.0) * 0.5; // Konvertujemo u opseg od 0 do 1

    vec4 redTint = vec4(1.0, 0.0, 0.0, pulse); // Crvena boja sa promenljivom transparentnoš?u

    outCol = texColor * redTint;
}
