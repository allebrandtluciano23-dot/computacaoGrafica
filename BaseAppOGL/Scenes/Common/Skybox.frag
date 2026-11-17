#version 330 core

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3  uFogColor;
uniform float uHorizonFog = 0.25;   // quanto do céu, a partir do horizonte, some na neblina

void main()
{
    vec3 sky = texture(skybox, TexCoords).rgb;

    // Perto do horizonte o céu se mistura com a neblina,
    // assim não aparece um "corte" entre o terreno e o céu
    float h = normalize(TexCoords).y;                 // -1 = embaixo, 0 = horizonte, 1 = topo
    float f = clamp(h / uHorizonFog, 0.0, 1.0);
    FragColor = vec4(mix(uFogColor, sky, f), 1.0);
}