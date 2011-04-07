//variying vec3 normal;
//const vec4 ambient0 = vec4(1.0, 1.0, 1.0, 1.0);
//const vec4 diffuse0 = vec4(1.0, 1.0, 1.0, 1.0);
//const vec4 specular0 = vec4(1.0, 1.0, 1.0, 1.0);
//const vec4 light0_Pos = vec4(0.0, 1.0, 1.0, 0.0);
uniform mat4 u_modelViewMatrix;
uniform mat4 u_projectionMatrix;

void main()
{
    gl_Position = u_projectionMatrix*u_modelViewMatrix*gl_Vertex;
    //normal = normalize(gl_NormalMatrix * gl_Normal);
}
