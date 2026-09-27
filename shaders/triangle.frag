#version 330 core

// Interpolated vertex position and the fragment's output color.
in vec3 vertexPosition;
out vec4 FragColor;

void main()
{
    // Build an opaque color gradient from the interpolated position.
    FragColor = vec4(vertexPosition.x + 0.5, vertexPosition.y + 0.5, vertexPosition.z, 1.0f);
}