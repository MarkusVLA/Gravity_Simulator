#version  330 core

attribute vec2 a_position;
attribute vec4 a_color;

varying vec4 v_color;

void main() {
    // Pass the color to the fragment shader
    v_color = a_color;  // sf::Color is already normalized

    // Set the position
    gl_Position = vec4(a_position, 0.0, 1.0);

    // Set the point size
    gl_PointSize = 10.0;  // Change this to the desired point size
}