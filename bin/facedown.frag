#version 330

out vec4 FragColor;

uniform vec3 gLightWorldPos;
uniform sampler2D gShadowMap0;
uniform sampler2D gShadowMap1;
uniform sampler2D gShadowMap2;
uniform sampler2D gShadowMap3;
uniform sampler2D gShadowMap4;

uniform sampler2D gNormalMap;
uniform sampler2D gColorMap;
uniform vec3 gEyeWorldPos;

in vec3 WorldPos0;
in vec3 Normal0;
in vec2 TexCoord0;
in vec3 Tangent0;

in vec4 LightSpacePos;

uniform float FogB;
uniform float FogC;

vec3 applyFog( in vec3  rgb, in float distance, in vec3  rayOri, in vec3  rayDir )
{
    float fogAmount = FogC * exp(-rayOri.y*FogB) * (1.0-exp( -distance*rayDir.y*FogB ))/rayDir.y;
    vec3  fogColor  = vec3(0.9, 0.9, 0.9);
    return mix( rgb, fogColor, fogAmount );
}

vec3 CalcBumpedNormal()
{
    vec3 Normal = normalize(Normal0);
    vec3 Tangent = normalize(Tangent0);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture(gNormalMap, TexCoord0).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
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
        return 0.5;
    } else {
        return 1.0;
	}
}

void main()
{
    const vec4 diffColor = vec4(0.5, 0.5, 0.5, 1.0);
	const vec4 ambientColor = texture2D(gColorMap, TexCoord0.xy);
	const float ambientIntensity = 0.5;
	
	vec4 amb = ambientColor * ambientIntensity;

	vec3 Normal = CalcBumpedNormal();
    vec3 n2 = normalize(Normal);
    vec3 l2 = normalize(gLightWorldPos - WorldPos0);
	
	vec4 diff = diffColor * max(dot(n2, l2), 0.0);

	float shad = (CalcShadowFactor(gShadowMap0, LightSpacePos) + CalcShadowFactor(gShadowMap1, LightSpacePos) + CalcShadowFactor(gShadowMap2, LightSpacePos) + CalcShadowFactor(gShadowMap3, LightSpacePos) + CalcShadowFactor(gShadowMap4, LightSpacePos)) / 5;

    FragColor =  shad * diff + amb;
	FragColor.rgb = applyFog(FragColor.rgb, distance(gEyeWorldPos, WorldPos0), gEyeWorldPos, WorldPos0 - gEyeWorldPos);
	FragColor.a = 1.0;
}