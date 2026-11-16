#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; //nova posicao
in mat3 TBN; //nova posicao do mapping
out vec4 FragColor;

uniform sampler2D Texture0;
uniform sampler2D NormalMap;         // NOVO: normal map
uniform bool  uUseNormalMap = false; // NOVO: liga/desliga
uniform float uNormalStrength = 1.0; // NOVO: força do relevo


/// iluminacao
uniform vec3 uViewPos; // posicao da cam
uniform vec3 uAmbient;  // a luz o ambient
uniform vec3 uLightPos; // posicao da luz
uniform vec3 uLightColor; // cor * a pos luz

// neblina
uniform vec3 uFogColor;
uniform float uFogDensity;


void main()
{
    vec4 texColor = texture(Texture0, TexCoord);

    texColor.rgb *= texColor.a;
    if(texColor.a < 0.1) // Adjust threshold as needed
        discard;


    vec3 N;
    if (uUseNormalMap)
    {
        vec3 n = texture(NormalMap, TexCoord).rgb;  // cor de 0..1
        n = n * 2.0 - 1.0;                          // direção de -1..1
        n.xy *= uNormalStrength;                    // exagera ou suaviza o relevo
        N = normalize(TBN * n);                     // gira para o mundo
    }
    else
    {
        N = normalize(Normal);
    }

    //                                               Bling phong
    vec3 toLight = uLightPos - FragPos;
    float dist = length(toLight);
    vec3 L = toLight / dist;

    // Difusa: quanto a superfície está virada para a luz
    float diff = max(dot(N, L), 0.0);

    // Especular: o brilho/reflexo
    vec3 V = normalize(uViewPos - FragPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), 32.0) * 0.2;

    // Atenuação: a luz enfraquece com a distância
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

    vec3 lighting = uAmbient + (diff + spec) * uLightColor * attenuation;
    vec3 color = texColor.rgb * lighting;
    

    //                                           Neblina (exponencial ao quadrado)   
    float camDist = length(uViewPos - FragPos);
    float fogFactor = exp(-pow(uFogDensity * camDist, 2.0));   // 1 = sem neblina, 0 = só neblina
    color = mix(uFogColor, color, clamp(fogFactor, 0.0, 1.0));

    FragColor = vec4(color, texColor.a);
}