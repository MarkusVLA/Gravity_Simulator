#version 120

uniform float u_speed; // Speed of the particle
uniform vec4 u_color; // Color of the particle

vec3 gradient(float t) {
    const vec3 c0 = vec3(0.267004, 0.004874, 0.329415);
    const vec3 c1 = vec3(0.283072, 0.130895, 0.449241);
    const vec3 c2 = vec3(0.262138, 0.242724, 0.520837);
    const vec3 c3 = vec3(0.220057, 0.343307, 0.549413);
    const vec3 c4 = vec3(0.153364, 0.445163, 0.557845);
    const vec3 c5 = vec3(0.119512, 0.534392, 0.555996);
    const vec3 c6 = vec3(0.166383, 0.690856, 0.496502);
    const vec3 c7 = vec3(0.319809, 0.770914, 0.411152);
    const vec3 c8 = vec3(0.525776, 0.833491, 0.288127);
    const vec3 c9 = vec3(0.762373, 0.876424, 0.137064);
    const vec3 c10 = vec3(0.993248, 0.906157, 0.143936);

    if (t < 0.1) return mix(c0, c1, t / 0.1);
    else if (t < 0.2) return mix(c1, c2, (t - 0.1) / 0.1);
    else if (t < 0.3) return mix(c2, c3, (t - 0.2) / 0.1);
    else if (t < 0.4) return mix(c3, c4, (t - 0.3) / 0.1);
    else if (t < 0.5) return mix(c4, c5, (t - 0.4) / 0.1);
    else if (t < 0.6) return mix(c5, c6, (t - 0.5) / 0.1);
    else if (t < 0.7) return mix(c6, c7, (t - 0.6) / 0.1);
    else if (t < 0.8) return mix(c7, c8, (t - 0.7) / 0.1);
    else if (t < 0.9) return mix(c8, c9, (t - 0.8) / 0.1);
    else return mix(c9, c10, (t - 0.9) / 0.1);
}

void main() {
    // Map the speed to a value between 0 and 1
    // This mapping depends on the expected range of particle speeds in your application
    float normalizedSpeed = clamp(u_speed, 0.0, 1.0);

    // Get the color from the gradient function based on the normalized speed
    vec3 color = gradient(normalizedSpeed);

    gl_FragColor = vec4(color, 1.0); // Use the gradient color with full alpha
}