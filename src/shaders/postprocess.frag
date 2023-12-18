#version 330
uniform sampler2D u_texture;
uniform vec2 u_texelSize;
uniform int u_radius;


float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

vec4 applyGaussianBlur(vec2 texCoords, float sigma) {
    vec4 sum = vec4(0.0);
    float totalWeight = 0.0;

    for (int x = -u_radius; x <= u_radius; ++x) {
        for (int y = -u_radius; y <= u_radius; ++y) {
            float weight = gaussian(length(vec2(x, y)), sigma);
            vec4 texColor = texture2D(u_texture, texCoords + vec2(x, y) * u_texelSize);
            sum += vec4(texColor.rgb * weight, weight);
            totalWeight += weight;
        }
    }

    return vec4(sum.rgb / totalWeight, sum.a / totalWeight);
}

vec3 gradient(float t) {
    const vec3 c0 = vec3(0.26700401, 0.00487433, 0.32941519);
    const vec3 c1 = vec3(0.993248, 0.906157, 0.143936);
    const vec3 c2 = vec3(-0.58068311, -0.58293401, -0.68376682);
    const vec3 c3 = vec3(0.33747528, 0.249201, 0.23484185);
    const vec3 c4 = vec3(-0.33825602, -0.316152, -0.21384384);

    return c0 + t * (c1 + t * (c2 + t * (c3 + t * c4)));
}

void main() {
    vec2 texCoords = gl_TexCoord[0].xy;
    vec4 sum = vec4(0.0);
    float totalWeight = 0.0;
    float sigma = 1.5;  // Adjusted sigma for smoother blur

    // Gaussian blur to calculate local density
    for (int x = -u_radius; x <= u_radius; ++x) {
        for (int y = -u_radius; y <= u_radius; ++y) {
            float weight = gaussian(length(vec2(x, y)), sigma);
            vec4 texColor = texture2D(u_texture, texCoords + vec2(x, y) * u_texelSize);
            sum += vec4(texColor.rgb * weight, weight);
            totalWeight += weight;
        }
    }

    vec4 blurColor = vec4(sum.rgb / totalWeight, sum.a / totalWeight);

    // Determine local density
    float density = length(blurColor.rgb);

    // Original color at the point
    vec4 originalColor = texture2D(u_texture, texCoords);

    // Determine final color based on density
    vec3 color;
    if (density > 0.02) {  // Slightly increased density threshold
        color = gradient(density);
    } else {
        color = originalColor.rgb;
    }

    gl_FragColor = vec4(color, 1.0);
}