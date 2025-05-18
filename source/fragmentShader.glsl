#version 330 core

in vec2 texCoords; // Texture coords
out vec3 colour;

uniform sampler2D texMap;

void main()
{
    colour = vec3(texture(texMap, texCoords));
}
