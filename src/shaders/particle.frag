#version 120

void main() {
    // Calculate the distance from the center of the particle
    float dist = length(gl_PointCoord - vec2(0.5));
    // Calculate the alpha value based on the distance
    float alpha = 1.0 - smoothstep(0.0, 0.5, dist);
    // Set the output color
    gl_FragColor = vec4(1.0, 0.0, 0.0, alpha); // Red color
}