#version 330 core
layout (location = 0) in vec4 vertex; // x,y Position. z,w texCoord

out vec4 ParticleColor;
out vec2 TexCoords;

uniform vec4 color;
uniform vec2 offset;
uniform mat4 projection;

void main()
{
    float scale = 10.0f;
    TexCoords = vertex.zw;
    ParticleColor = color;
    gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}