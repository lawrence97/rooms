#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec4 col;

uniform vec2 window_scale = {1.0f, 1.0f};
uniform vec2 camera_position = {1.0f, 1.0f};
uniform vec2 camera_scale = {1.0f, 1.0f};

varying vec2 pass_tex;
varying vec4 pass_col;
    
vec2 result_position;

 void main() {
    pass_tex = tex;
    pass_col = col;
         
    result_position.x = pos.x * camera_scale.x * (2.0f / window_scale.x);
    result_position.y = pos.y * camera_scale.y * (2.0f / window_scale.y);
     
    result_position.x = result_position.x + (camera_position.x / window_scale.x);
    result_position.y = result_position.y + (camera_position.y / window_scale.y);
    
    gl_Position = vec4(result_position.x, result_position.y, 1.0f, 1.0f);
 }
