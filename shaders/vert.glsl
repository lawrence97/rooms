#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec4 col;

varying vec2 pass_tex;
varying vec4 pass_col;

 void main() {
     pass_tex = tex;
     pass_col = col;
     gl_Position = vec4(pos.x / 320.0f, pos.y / 320.0f, 1.0f, 1.0f);
 }
