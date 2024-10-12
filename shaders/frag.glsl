#version 330 core

varying vec2 pass_tex;
varying vec4 pass_col;

out vec4 colour;

void main() {
    colour = pass_col;
}
