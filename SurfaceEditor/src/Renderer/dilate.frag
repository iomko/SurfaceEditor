#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float radius;
uniform float gridX; // 1/screenWidth
uniform float gridY; // 1/screenHeight

void main() {
  for (float i = -radius; i < radius; ++i) {
    for (float j = -radius; j < radius; ++j) {
      if (i * i + j * j < (1e-6 + radius * radius)) { // inside a circle
        vec2 uv = TexCoords.st + vec2(i * gridX, j * gridY);
        vec3 colour = texture(screenTexture, uv).rgb;
        if (colour.r + colour.g + colour.b > 0) {
          FragColor = vec4(colour, 1.F);
          return;
        }
      }
    }
  }
  // not near the selected object, discard
  discard;
}