#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

uniform vec4 loc;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
  gl_Position = vec4(position, 1.0f) + loc;
  ourColor = color;
  TexCoord = texCoord;
}
