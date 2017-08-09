#version 330

layout (location = 0) in vec3 Position;
layout (location = 2) in vec3 Normal;

out vec3 WorldPos0;
out vec3 Normal0;

uniform mat4 gWorld;
uniform mat4 gCamera;
uniform mat4 gProjection;

void main()
{
    gl_Position = gProjection * gCamera * gWorld * vec4(Position, 1.0);
	Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;
	WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;
}