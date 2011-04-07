uniform mat4 u_modelViewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec4 u_light_ambient;
uniform vec4 u_light_diffuse;
uniform vec4 u_light_specular;
uniform vec4 u_light_pos;

void main()
{
    vec3 normal, lightDir, halfVector;
    vec4 diffuse, ambient, specular;
    mat3 normalMatrix;
    normalMatrix[0] = vec3(u_modelViewMatrix[0]);
    normalMatrix[1] = vec3(u_modelViewMatrix[1]);
    normalMatrix[2] = vec3(u_modelViewMatrix[2]);

    normal = normalize(normalMatrix * gl_Normal);
    lightDir = normalize(u_light_pos.xyz);
    halfVector = normalize(lightDir + vec3(0, 0, 1));

    ambient = gl_FrontMaterial.ambient * u_light_ambient;
    diffuse = max(dot(normal, lightDir), 0.0) * gl_FrontMaterial.diffuse * u_light_diffuse;
    specular = pow(max(dot(normal, halfVector), 0.0), gl_FrontMaterial.shininess)
        * gl_FrontMaterial.specular * u_light_specular;

    gl_FrontColor = ambient + diffuse + specular;
    gl_Position = u_projectionMatrix * u_modelViewMatrix * gl_Vertex;
}
