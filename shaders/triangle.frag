#version 330 core

// Interpolated vertex position and the fragment's output color.
in vec3 vertexPosition;
out vec4 FragColor;

void main()
{
    // Map local positions from [-0.5, 0.5] to RGB colors in [0.0, 1.0].
    FragColor = vec4(vertexPosition + vec3(0.5), 1.0);
}