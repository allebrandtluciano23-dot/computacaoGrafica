#version 330 core

in vec3 FragPos;
out vec4 FragColor;

uniform samplerCube skybox;        // o mesmo cubemap do céu (environment map)
uniform vec3  uViewPos;            // posição da câmera
uniform float uTime;               // tempo em segundos (anima as ondinhas)

// Point light (mesma do terreno) - aparece como reflexo laranja na água
uniform vec3 uLightPos;
uniform vec3 uLightColor;

// Neblina (mesma do terreno)
uniform vec3  uFogColor;
uniform float uFogDensity;

// Aparência da água
uniform vec3  uWaterColor    = vec3(0.03, 0.07, 0.08);  // cor da água "por dentro"
uniform float uRefractRatio  = 1.0 / 1.33;              // ar (1.00) -> água (1.33)
uniform float uWaveStrength  = 1.0;

// Heightmap do terreno: usado para saber a profundidade da água em cada ponto
uniform sampler2D uHeightMap;
uniform float uTerrainSize = 256.0;   // o terreno vai de -128 a +128

void main()
{
    // ---------- Ondinhas: perturba a normal com senos que andam no tempo ----------
    vec2  p = FragPos.xz;
    float t = uTime;
    float nx = 0.040 * cos(p.x * 1.3 + t * 1.1) + 0.025 * cos((p.x + p.y) * 2.3 + t * 1.7);
    float nz = 0.040 * cos(p.y * 1.6 + t * 0.9) + 0.025 * cos((p.x - p.y) * 2.7 + t * 1.3);
    vec3 N = normalize(vec3(nx * uWaveStrength, 1.0, nz * uWaveStrength));

    // Direção do olho até o ponto da água
    vec3 I = normalize(FragPos - uViewPos);

    // ---------- REFLEXÃO: o raio "quica" na água e busca o céu ----------
    vec3 R = reflect(I, N);
    vec3 reflColor = texture(skybox, R).rgb;

    // ---------- REFRAÇÃO: o raio entra na água e entorta (lei de Snell) ----------
    vec3 T = refract(I, N, uRefractRatio);
    vec3 refrColor = mix(uWaterColor, texture(skybox, T).rgb, 0.25);

    // ---------- Fresnel: de cima vê-se mais o fundo, de lado vê-se mais o reflexo ----------
    float cosTheta = max(dot(-I, N), 0.0);
    float fresnel  = 0.08 + 0.92 * pow(1.0 - cosTheta, 5.0);

    vec3 color = mix(refrColor, reflColor, fresnel);

    // ---------- Brilho da point light refletido na água ----------
    vec3  toLight = uLightPos - FragPos;
    float dist = length(toLight);
    vec3  L = toLight / dist;
    vec3  H = normalize(L - I);
    float spec = pow(max(dot(N, H), 0.0), 120.0);
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
    color += uLightColor * spec * attenuation * 3.0;

    // ---------- Neblina ----------
    float camDist   = length(uViewPos - FragPos);
    float fogFactor = exp(-pow(uFogDensity * camDist, 2.0));
    color = mix(uFogColor, color, clamp(fogFactor, 0.0, 1.0));

    // ---------- Profundidade: altura da água - altura do terreno embaixo dela ----------
    vec2  huv = (FragPos.xz + uTerrainSize * 0.5 + 0.5) / uTerrainSize;
    float terrainY = texture(uHeightMap, huv).r * 255.0 * 0.25 - 16.0;  // mesma conta da CTerrain
    float depth = FragPos.y - terrainY;

    // ---------- TRANSPARÊNCIA ----------
    float alpha = mix(0.40, 0.92, fresnel);       // de cima: transparente / de lado: espelho
    alpha *= smoothstep(0.0, 0.6, depth);         // margem: some suavemente (sem "corte" na borda)
    color = mix(color, color * 0.6, smoothstep(0.3, 1.0, depth));  // água funda um pouco mais escura

    FragColor = vec4(color, alpha);
}
