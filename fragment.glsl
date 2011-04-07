uniform mat4 u_textureMatrix;
uniform int u_has_texture;
uniform sampler2D u_material_texture;

void main()
{
    vec4 texCoord = u_textureMatrix * gl_TexCoord[0];
    if(u_has_texture != 0)
        gl_FragColor = gl_Color * texture2D(u_material_texture, texCoord.xy);
    else
        gl_FragColor = gl_Color;
}
