#version 430 core
in vec3 ourColor;
// in vec2 TexCoord;

out vec3 color;

// uniform sampler2D ourTexture;

void main() {
  color = ourColor; 
}
