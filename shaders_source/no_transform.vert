#version 330
layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec3 vTangent;
layout (location=3) in vec2 vTexCoord;

out vec2 TexCoord;
void main() {
    // Don't move the vertex at all; assume its local space == clip space.
    gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1) ;
    TexCoord = vTexCoord;
}