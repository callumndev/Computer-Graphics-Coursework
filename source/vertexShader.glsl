#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 inputTextCoords;

out vec2 texCoords;

uniform mat4 MVP;

void main()
{
    // Set vertex position
    gl_Position = MVP * vec4(pos, 1.0);
    
    // Set output texture coords from input
    texCoords = inputTextCoords;
}
