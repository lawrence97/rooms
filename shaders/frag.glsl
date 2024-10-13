#version 330 core

varying vec2 pass_tex;
varying vec4 pass_col;

uniform sampler2D tex_sampler;

out vec4 colour;

void main() {
    colour = texture(tex_sampler, pass_tex).rgba * pass_col;
}
