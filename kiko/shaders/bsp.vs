/*
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
*/

#version 330

layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;

attribute vec3 coord3d;
attribute vec3 v_color;
uniform mat4 mvp;
varying vec3 pass_color;
varying vec3 f_color;

void main(void) {
  gl_Position = mvp * vec4(coord3d, 1.0);
  pass_color = v_color;
  f_color = v_color;
  TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}