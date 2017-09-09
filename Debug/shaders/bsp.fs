/*
#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;

void main()
{
    color = texture(ourTexture1, TexCoord);
}
*/

#version 330

in vec2 TexCoord;

uniform sampler2D ourTexture1;
varying vec3 pass_color;
out vec4 f_color;

void main(void) { 
	f_color = vec4(pass_color, 1);
}