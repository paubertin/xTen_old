#version 330 core

layout (location = 0) in vec3 Position;

uniform mat4 pvm;

void main()
{
    gl_Position = pvm * vec4(Position, 1.0f);
}