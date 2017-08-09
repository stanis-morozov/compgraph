#version 330

out vec4 FragColor;
in vec2 TexCoord0;
uniform sampler2D gSampler;

uniform vec3 gEyeWorldPos;
uniform vec3 gLightWorldPos;

uniform sampler2D gShadowMap0;
uniform sampler2D gShadowMap1;
uniform sampler2D gShadowMap2;
uniform sampler2D gShadowMap3;
uniform sampler2D gShadowMap4;

in vec3 WorldPos0;
in vec3 Normal0;

in vec4 LightSpacePos;

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
    float depth = texture(gShadowMap, UVCoords).x;
    if (depth < (z - 0.0002)) {
        return 0.5;
    } else {
        return 1.0;
	}
}

   
float halfLambert(vec3 vect1, vec3 vect2 )  
{  
     return dot (vect1, vect2) * 0.5 + 2.0;
}  
   
float blinnPhongSpecular(vec3 normalVec, vec3 lightVec, float specPower)  
{  
     vec3 halfAngle = normalize(normalVec + lightVec); 
	 
     return pow(clamp(0.0, 1.0, dot(normalVec, halfAngle)), specPower);
}  
   
void main()  
{ 
	const float matThickness = 3.0;
	const vec3 extinction = vec3(0.5, 0.5, 0.5);
	const vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 baseColor = texture2D(gSampler, TexCoord0.xy);
	const vec4  specColor = vec4(1.0, 1.0, 1.0, 1.0);
	const float specPower = 20.0;
	const float rimScalar = 20.0;

    float attenuation = 1.0 / distance(gLightWorldPos, WorldPos0);
    vec3 e = normalize(gEyeWorldPos - WorldPos0); 
    vec3 l = normalize(gLightWorldPos - WorldPos0); 
    vec3 nn = normalize(Normal0);
    vec4 ln = baseColor * attenuation * halfLambert (l, nn);
	float inFactor = max (0.0, dot(-nn, l)) + halfLambert (-e, l);
    vec3 indirect = vec3(matThickness * inFactor * attenuation) * extinction;
    vec3 rim = vec3(1.0 - max (0.0, dot(nn, e)));
	 
    rim *= rim;  
    rim *= max(0.0, dot(nn, l)) * specColor.rgb;  
   
    vec4 color = ln + vec4 (indirect, 1.0);  
	 
    color.rgb += (rim * rimScalar * attenuation);  
    color.rgb += vec3(blinnPhongSpecular(nn, l, specPower) * attenuation * specColor * 0.3);
   
    gl_FragColor = (CalcShadowFactor(gShadowMap0, LightSpacePos) + CalcShadowFactor(gShadowMap1, LightSpacePos) + CalcShadowFactor(gShadowMap2, LightSpacePos) + CalcShadowFactor(gShadowMap3, LightSpacePos) + CalcShadowFactor(gShadowMap4, LightSpacePos)) / 5 * color;
	gl_FragColor.rgb = applyFog(gl_FragColor.rgb, distance ( gEyeWorldPos, WorldPos0 ), gEyeWorldPos, WorldPos0 - gEyeWorldPos);
	gl_FragColor.a = 1.0;
}