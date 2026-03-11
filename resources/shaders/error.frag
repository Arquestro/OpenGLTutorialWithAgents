#version 330 core
in vec3 v_color;
out vec4 frag_color;
uniform float U_TIME_DATA;
void main()
{
    float checker_x = floor(gl_FragCoord.x / 16.0);
    float checker_y = floor(gl_FragCoord.y / 16.0);
    float checker = mod(checker_x + checker_y, 2.0);
    float pulse = 0.5 + 0.5 * sin(U_TIME_DATA * 4.0);
    vec3 magenta = vec3(1.0, 0.0, 1.0);
    vec3 black = vec3(0.05, 0.02, 0.05);
    vec3 checker_color = mix(magenta, black, checker);
    vec3 loud_color = mix(checker_color, vec3(1.0, 0.6, 1.0), pulse * 0.25);
    frag_color = vec4(loud_color, 1.0);
}
