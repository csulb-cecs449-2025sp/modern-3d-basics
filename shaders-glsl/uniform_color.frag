#version 330
layout (location=0) out vec4 FragColor;
uniform vec3 color;
void main() {
    // Make everything a uniform color.
    FragColor = vec4(color, 1.0);
}