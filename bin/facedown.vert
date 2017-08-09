#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;

out vec3 WorldPos0;
out vec3 Normal0;
out vec2 TexCoord0;
out vec3 Tangent0;

uniform mat4 gWorld;
uniform mat4 gCamera;
uniform mat4 gLight;
uniform mat4 gProjection;

out vec4 LightSpacePos;

void main()
{
	LightSpacePos = gProjection * gLight * gWorld * vec4(Position, 1.0);
	Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;
	WorldPos0   = (gWorld * vec4(Position, 1.0)).xyz;
    gl_Position = gProjection * gCamera * gWorld * vec4(Position, 1.0);
	TexCoord0 = TexCoord;
	Tangent0 = (gWorld * vec4(Tangent, 0.0)).xyz;
}