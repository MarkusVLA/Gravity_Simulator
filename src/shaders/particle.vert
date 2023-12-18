// vertex_shader.glsl
#version 120

attribute vec3 a_position; // Particle position

uniform mat4 u_viewMatrix; // Camera view matrix
uniform mat4 u_projectionMatrix; // Camera projection matrix

void main() {
    gl_Position = u_projectionMatrix * u_viewMatrix * vec4(a_position, 1.0);
}
