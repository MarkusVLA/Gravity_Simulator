#version 120

attribute vec2 a_position;
attribute vec4 a_color;

varying vec4 v_color;

void main() {
    // Pass the color to the fragment shader
    v_color = a_color / 255.0;  // Convert from sf::Color to vec4

    // Set the position
    gl_Position = vec4(a_position, 0.0, 1.0);
}