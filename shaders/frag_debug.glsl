#version 330 core

varying vec4 pass_col;

out vec4 colour;

void main() {
    colour = pass_col;
}
