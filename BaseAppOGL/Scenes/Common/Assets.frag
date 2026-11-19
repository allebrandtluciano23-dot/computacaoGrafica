#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; //nova posicao
in mat3 TBN; //nova posicao do mapping
in vec2 BaseUV; // sem repeticao pra mascara calcario 
out vec4 FragColor;

uniform sampler2D Texture0;
uniform sampler2D NormalMap;         // folhas normal map
uniform sampler2D NormalMap1;       // cascalho normal map
uniform bool  uUseNormalMap = false; 
uniform float uNormalStrength = 1.0; // força do relevo

uniform sampler2D Texture1; // cascaljho
uniform sampler2D MaskMap; // nova mascara
uniform bool  uUseMultiTex = false; 
uniform float uGravelScale = 3.0; 
uniform float uGravelBrightness = 0.7;   //escurece o cascalho para combinar com as folhas
uniform float uWaterLevel = -1000.0; // nivel da agua (para nao pintar o fundo do lago)
uniform vec2  uLakeCenter = vec2(-13.0, 9.0);   // centro do lago (x, z)
uniform float uLakeRadius = 11.0;               // até onde vai o efeito de fundo de lago


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
   vec4 texColor = texture(Texture0, TexCoord);   // folhas

    // ---------- Multi-textura ----------
    float mask = 0.0;
    if (uUseMultiTex)
    {
        vec4 tex2 = texture(Texture1, TexCoord * uGravelScale);   // cascalho (repetido)
        tex2.rgb *= uGravelBrightness;
         tex2.rgb *= vec3(1.0, 0.9, 0.75); //cor cacalhos
        mask = texture(MaskMap, BaseUV).r;         // máscara (sem repetir)
        texColor = mix(texColor, tex2, mask);      // 0 = folhas, 1 = cascalho
         
        //coisa agua
        float nearLake = 1.0 - smoothstep(uLakeRadius - 2.0, uLakeRadius, distance(FragPos.xz, uLakeCenter));
        float underwater = smoothstep(uWaterLevel + 0.3, uWaterLevel - 0.2, FragPos.y) * nearLake;
        texColor = mix(texColor, tex2, underwater);
        mask = max(mask, underwater);
        float depthDark = smoothstep(uWaterLevel, uWaterLevel - 1.0, FragPos.y);
        texColor.rgb *= mix(0.75, 0.35, depthDark) * underwater + (1.0 - underwater);
    }

    texColor.rgb *= texColor.a;
    if(texColor.a < 0.1) // Adjust threshold as needed
        discard;


    vec3 N;
    if (uUseNormalMap)
    {
        vec3 nFolhas = texture(NormalMap,  TexCoord).rgb * 2.0 - 1.0;  // relevo das folhas
        vec3 nPedras = texture(NormalMap1, TexCoord * uGravelScale).rgb * 2.0 - 1.0;  // relevo do cascalho
        vec3 n = normalize(mix(nFolhas, nPedras, mask));        
        n.xy *= uNormalStrength;     
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