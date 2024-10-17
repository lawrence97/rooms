#version 330 core

layout (location = 0) in vec2 pos;

uniform vec4 uni_col;

varying vec4 pass_col;

 void main() {
     pass_col = uni_col;
     gl_Position = vec4(pos.x / 320.0f, pos.y / 320.0f, 1.0f, 1.0f);
 }
