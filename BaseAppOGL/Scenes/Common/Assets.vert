#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos; //nova posicao 
out mat3 TBN; //nova posicao do mapping 


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uTiling = 1.0;

void main()
{
    TexCoord = texCoord* uTiling;
    FragPos = vec3(model * vec4(position, 1.0f)); //nova posicao
   
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    vec3 N = normalize(normalMatrix * normal);
    Normal = N;

    // ---- Monta a base TBN (Tangent, Bitangent, Normal) ----
    vec3 T, B;
    if (length(tangent) > 0.001)
    {
        // Modelos (.obj/.3ds) já trazem tangente e bitangente
        T = normalize(normalMatrix * tangent);
        B = normalize(normalMatrix * bitangent);
    }
    else
    {
        // O terreno não traz tangente: calculamos aqui.
        // No terreno, a coordenada U da textura cresce no eixo +X.
        T = normalize(vec3(1.0, 0.0, 0.0) - N * N.x);
        B = normalize(cross(T, N));
    }
    TBN = mat3(T, B, N);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}