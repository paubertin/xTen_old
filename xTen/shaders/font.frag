#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D tex;
uniform vec4 textColor;

void main()
{    
    //vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    //color = textColor * sampled;
	//color = texture2D(text, TexCoords) * textColor;
	color = vec4(textColor.rgb, texture2D(tex, TexCoords).a * textColor.a);
}  