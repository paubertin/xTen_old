#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 pvm;
uniform mat4 model;

out DATA
{
	vec2 texcoord;
	vec3 normal;
	vec3 worldpos;
} shader_out;

void main()
{
    gl_Position = pvm * vec4(Position, 1.0f);
	shader_out.texcoord = TexCoord;
	shader_out.normal = (transpose(inverse(model)) * vec4(Normal, 0.f)).xyz;
	shader_out.worldpos = (model * vec4(Position, 1.0f)).xyz;
}