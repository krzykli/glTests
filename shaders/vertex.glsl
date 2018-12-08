#version 330

layout (location = 0) in vec3 pos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec4 vCol;
uniform float time;

void main()
{
    float sec = (sin(time * 2) + 1) / 2.0;
    vec3 newPos= vec3(pos.x - sec, pos.y, pos.z);
    gl_Position = projection * view * model * vec4(newPos, 1.0);
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0);
}
