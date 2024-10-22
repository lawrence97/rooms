#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec4 col;

uniform vec2 window_scale = {1.0f, 1.0f};

varying vec2 pass_tex;
varying vec4 pass_col;

 void main() {
     pass_tex = tex;
     pass_col = col;
     gl_Position = vec4(0.1f, 0.1f, 1.0f, 1.0f) * vec4(pos.x / window_scale.x, pos.y / window_scale.y, 1.0f, 1.0f);
 }
