#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos; //nova posicao 


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uTiling = 1.0;

void main()
{
    TexCoord = texCoord* uTiling;

    Normal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * vec4(position, 1.0f)); //nova posicao

    gl_Position = projection * view * model * vec4(position, 1.0f);
}