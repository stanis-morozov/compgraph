#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

out vec2 TexCoord0;

uniform mat4 gWorld;
uniform mat4 gCamera;
uniform mat4 gProjection;

void main()
{
    gl_Position = gProjection * gCamera * gWorld * vec4(Position, 1.0);
	TexCoord0 = TexCoord;
}