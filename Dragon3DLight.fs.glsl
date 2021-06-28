varying vec3 v_normal;

const vec3 L = vec3(0.0, 0.0, 1.0);

varying vec2 v_texcoords;
uniform sampler2D u_sampler;

vec3 Diffuse(vec3 MatColor, vec3 N, vec3 L)
{
    float Lambert = max( dot(N, L), 0);
    return MatColor * max(Lambert, 0.0);
}

void main(void)
{
//    vec3 diffuseColor = Diffuse(vec3(0.2, 0.2, 0.2), v_normal, L);
//    gl_FragColor = vec4(vec3(0.2, 0.2, 0.2), 1.0);
    vec4 color = texture2D(u_sampler, v_texcoords);
    gl_FragColor = color;
}