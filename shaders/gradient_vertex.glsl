
#version 330

layout (location = 0) in vec3 pos;
out vec4 vCol;

void main()
{
    gl_Position = vec4(pos, 1.0);
    vCol = vec4(pos, 1.0);
}
