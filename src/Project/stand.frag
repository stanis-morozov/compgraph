#version 330

out vec4 FragColor;

uniform vec3 gEyeWorldPos;
uniform vec3 gLightWorldPos;

uniform sampler2D gShadowMap0;
uniform sampler2D gShadowMap1;
uniform sampler2D gShadowMap2;
uniform sampler2D gShadowMap3;
uniform sampler2D gShadowMap4;
in vec4 LightSpacePos;

in vec3 WorldPos0;
in vec3 Normal0;

uniform float FogB;
uniform float FogC;

vec3 applyFog(vec3 rgb, float distance, vec3 rayOri, vec3 rayDir)
{
    float fogAmount = FogC * exp(-rayOri.y * FogB) * (1.0 - exp(-distance * rayDir.y * FogB)) / rayDir.y;
    vec3 fogColor = vec3(0.9, 0.9, 0.9);
    return mix(rgb, fogColor, fogAmount);
}

float CalcShadowFactor(sampler2D gShadowMap, vec4 LightSpacePos)
{
    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z= 0.5 * ProjCoords.z + 0.5;
    float Depth = texture(gShadowMap, UVCoords).x;
    if (Depth < (z - 0.0001)) {
        return 0.2;
    } else {
        return 1.0;
	}
}

void main()
{
    const vec4 diffColor = vec4 (0.82, 0.906, 0.984, 1.0);
    const vec4 specColor = vec4 (1.0, 1.0, 1.0, 1.0);
    const float k = 10.0;
	const vec4 ambientColor = vec4(0.82, 0.906, 0.984, 1.0);
	const float ambientIntensity = 0.5;
	
	vec4 amb = ambientColor * ambientIntensity;

    vec3 n2 = normalize(Normal0);
    vec3 l2 = normalize(gLightWorldPos - WorldPos0);
	vec3 v2 = normalize(gEyeWorldPos - WorldPos0);
    vec3 h2 = normalize(l2 + v2);
    
	float hn = dot(h2, n2);
    float hn2  = hn * hn;

    vec4  diff = diffColor * max(dot(n2, l2), 0.0);
    vec4  spec = specColor * exp(-k * (1.0 - hn2) / hn2);

	float shad = (CalcShadowFactor(gShadowMap0, LightSpacePos) + CalcShadowFactor(gShadowMap1, LightSpacePos) + CalcShadowFactor(gShadowMap2, LightSpacePos) + CalcShadowFactor(gShadowMap3, LightSpacePos) + CalcShadowFactor(gShadowMap4, LightSpacePos)) / 5;

    gl_FragColor = shad * (diff + spec) + amb;
	gl_FragColor.rgb = applyFog(gl_FragColor.rgb, distance(gEyeWorldPos, WorldPos0), gEyeWorldPos, WorldPos0 - gEyeWorldPos);
	gl_FragColor.a = 1.0;
}