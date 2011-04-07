uniform mat4 u_modelViewMatrix;
uniform mat4 u_projectionMatrix;
const vec4 ambient0 = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 diffuse0 = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 specular0 = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 light0_Pos = vec4(0.0, 1.0, 1.0, 0.0);

void main()
{
    vec3 normal, lightDir, halfVector;
    vec4 diffuse, ambient, specular;
    mat3 normalMatrix;
    normalMatrix[0] = vec3(u_modelViewMatrix[0]);
    normalMatrix[1] = vec3(u_modelViewMatrix[1]);
    normalMatrix[2] = vec3(u_modelViewMatrix[2]);

    normal = normalize(normalMatrix * gl_Normal);
    lightDir = normalize(light0_Pos.xyz);
    halfVector = normalize(lightDir + vec3(0, 0, 1));

    ambient = gl_FrontMaterial.ambient * ambient0;
    diffuse = max(dot(normal, lightDir), 0.0) * gl_FrontMaterial.diffuse * diffuse0;
    specular = pow(max(dot(normal, halfVector), 0.0), gl_FrontMaterial.shininess)
        * gl_FrontMaterial.specular * specular0;

    gl_FrontColor = ambient + diffuse + specular;
    gl_Position = u_projectionMatrix * u_modelViewMatrix * gl_Vertex;
}
