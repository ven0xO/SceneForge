#version 330 core     
// Position input at attribute 0 and output for interpolated coloring.
layout(location = 0) in vec3 aPos;

out vec3 vertexPosition;

void main()
{
    // Keep the local position for the fragment color.
    vertexPosition = aPos;

    // Use a fixed rotation to show three faces of the cube.
    float angleX = radians(-25.0);
    float angleY = radians(35.0);
    float cosX = cos(angleX);
    float sinX = sin(angleX);
    float cosY = cos(angleY);
    float sinY = sin(angleY);

    // GLSL matrix constructors take columns, not rows.
    mat3 rotationX = mat3(
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, cosX, sinX),
        vec3(0.0, -sinX, cosX)
    );
    mat3 rotationY = mat3(
        vec3(cosY, 0.0, -sinY),
        vec3(0.0, 1.0, 0.0),
        vec3(sinY, 0.0, cosY)
    );

    // Rotate around Y, then X, and use the result directly in clip space.
    gl_Position = vec4(rotationX * rotationY * aPos, 1.0);
}