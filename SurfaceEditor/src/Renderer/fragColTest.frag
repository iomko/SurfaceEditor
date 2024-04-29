
/*
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D screenTexture1;

void main() {
    // Sample the textures
    vec4 colorA = texture(screenTexture, TexCoords);
    vec4 colorC = texture(screenTexture1, TexCoords);

    // Define the conditions for outputting color
    bool conditionA = colorA == vec4(1.0f, 0.5f, 0.0f, 1.0f); // Orange color in targetA
    bool conditionC = colorC == vec4(0.0, 0.0, 0.0, 1.0); // Black color in targetC

    // If both conditions are met, output color, otherwise output transparent
    if (conditionA && conditionC) {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // White color
    } else {
        discard; // Discard the fragment
    }
}
*/


#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D screenTexture1;

void main() {
    // Sample the textures
    vec4 colorA = texture(screenTexture, TexCoords);
    vec4 colorC = texture(screenTexture1, TexCoords);

    // Define the condition for outputting color
    float sumColorC = colorC.r + colorC.g + colorC.b;
    // Define the condition for outputting color
    float sumColorA = colorA.r + colorA.g + colorA.b;

    bool conditionC = sumColorC > 0.0; // Check if the sum of r, g, and b is greater than 0
    bool conditionA = sumColorA > 0.0; // Check if the sum of r, g, and b is greater than 0

    // If both conditions are met, output color, otherwise output transparent
    if (conditionA && !conditionC) {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // White color
    } else {
        discard; // Discard the fragment
    }
}
