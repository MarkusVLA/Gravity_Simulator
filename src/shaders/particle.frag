#version 330 core

uniform vec4 u_color;

void main() {
    // Use the color from the uniform
    gl_FragColor = u_color;
}