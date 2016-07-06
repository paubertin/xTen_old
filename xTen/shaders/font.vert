#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * position;
    TexCoords = texCoord;
}  