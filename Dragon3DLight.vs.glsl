attribute vec4 a_position;  // (x, y, z, w=1)
attribute vec2 a_texcoords;
attribute vec3 a_normal;

uniform mat4 u_rotationMatrix;
uniform mat4 u_translationMatrix;
uniform mat4 u_projectionMatrix;

varying vec3 v_normal;
varying vec2 v_texcoords;

void main(void)
{
    mat4 worldMatrix = u_translationMatrix * u_rotationMatrix;

    v_normal = mat3(worldMatrix) * a_normal;

    gl_Position = u_projectionMatrix * worldMatrix * a_position;

    v_texcoords = a_texcoords;
}