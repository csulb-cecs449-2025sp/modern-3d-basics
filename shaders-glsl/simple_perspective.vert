#version 330
layout (location=0) in vec3 vPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    // Project the position to clip space.
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
}