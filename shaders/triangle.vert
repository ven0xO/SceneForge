#version 330 core     
// Position input at attribute 0 and output for interpolated coloring.
layout(location = 0) in vec3 aPos;

out vec3 vertexPosition;

void main()
{
    // Forward the position for coloring and use it directly in clip space.
    vertexPosition = aPos;
    gl_Position = vec4(aPos, 1.0f);
}