uniform int u_has_texture;
uniform sampler2D u_material_texture;

void main()
{
    gl_FragColor = gl_Color;
    if(u_has_texture != 0)
        gl_FragColor = gl_FragColor * texture2D(u_material_texture, gl_TexCoord[0].st);
}
