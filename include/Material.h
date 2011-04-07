#ifndef INITIALS_MATERIAL_H
#define INITIALS_MATERIAL_H

#include <QString>
#include "Vertex.h"

class Material
{
public:
    Material();
    Material(vec4 ambient, vec4 diffuse, vec4 specular, float shine);

    const vec4 & ambient() const;
    const vec4 & diffuse() const;
    const vec4 & specular() const;
    float shine() const;
    void setAmbient(const vec4 &ambient);
    void setDiffuse(const vec4 &diffuse);
    void setSpecular(const vec4 &specular);
    void setShine(float shine);

    uint texture() const;
    void setTexture(uint texture);
    void freeTexture();

    void loadTextureTIFF(QString path, bool mipmaps = false);

private:
    // Create a texture from a TIFF file using libtiff
    static uint textureFromTIFFImage(QString path, bool mipmaps);
    static void setTextureParams(uint target, bool mipmaps);

    vec4 m_ambient;
    vec4 m_diffuse;
    vec4 m_specular;
    float m_shine;
    uint m_texture;
};

#endif
