#version 330

out vec4 FragColor;

uniform vec3 gEyeWorldPos;
uniform vec3 gLightWorldPos;

in vec3 WorldPos0;
in vec3 Normal0;

uniform float FogB;
uniform float FogC;

vec3 applyFog(vec3 rgb, float distance, vec3 rayOri, vec3 rayDir)
{
    float fogAmount = FogC * exp(-rayOri.y * FogB) * (1.0 - exp(-distance * rayDir.y * FogB )) / rayDir.y;
    vec3 fogColor = vec3(0.9, 0.9, 0.9);
    return mix(rgb, fogColor, fogAmount);
}

void main()
{
    const vec4  diffColor = vec4 (1.0, 1.0, 1.0, 1.0);
    const vec4  specColor = vec4 (0.7, 0.7, 0.0, 1.0);
    const float k = 10.0;
	const vec4 ambientColor = vec4(0.8, 0.8, 0.8, 1.0);
	const float ambientIntensity = 0.5;
	
	vec4 amb = ambientColor * ambientIntensity;

    vec3 n2 = normalize(Normal0);
    vec3 l2 = normalize(gLightWorldPos - WorldPos0);
	vec3 v2 = normalize(gEyeWorldPos - WorldPos0);
    vec3 h2 = normalize(l2 + v2);
    
	float hn = dot(h2, n2);
    float hn2 = hn * hn;

    vec4 diff = diffColor * max(dot(n2, l2), 0.0);
    vec4 spec = specColor * exp(-k * (1.0 - hn2) / hn2);

    gl_FragColor = diff + spec + amb;
	gl_FragColor.rgb = applyFog(gl_FragColor.rgb, distance(gEyeWorldPos, WorldPos0), gEyeWorldPos, WorldPos0 - gEyeWorldPos);
	gl_FragColor.a = 1.0;
}