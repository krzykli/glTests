#version 330

out vec4 color;
in vec4 vCol;

uniform vec2 resolution;
uniform vec2 mousePos;
uniform float time;

void main()
{
    float s = gl_FragCoord.x / resolution.x;
    float t = gl_FragCoord.y / resolution.y;

    float sec = (sin(time * 2) + 1) / 2.0;

    vec3 c1 = vec3(0.4, 0.6, 0.8);
    vec3 c2 = vec3(0.0, 0.1, 0.2);
    vec3 c3 = vec3(sec, 0.0, 0.0);

    color = vec4(t * c1 + (1.0 - t) * c2, 1.0);
}
