#version 330 core
in vec3 v_color;
out vec4 frag_color;
uniform float U_TIME_DATA;
void main()
{
    float time_seconds = U_TIME_DATA;
    float pulse = 0.5 + 0.5 * sin(time_seconds * 2.0);
    vec3 animated_color = mix(v_color * 0.35, v_color, pulse);
    frag_color = vec4(animated_color, 1.0);
}
