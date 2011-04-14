#ifndef INITIALS_MATERIAL_H
#define INITIALS_MATERIAL_H

#include <string>
#include <inttypes.h>
#include "Vertex.h"

using namespace std;

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

    uint32_t texture() const;
    void setTexture(uint32_t texture);
    void freeTexture();

    void loadTextureTIFF(string path, bool mipmaps = false);

private:
    // Create a texture from a TIFF file using libtiff
    static uint32_t textureFromTIFFImage(string path, bool mipmaps);
    static void setTextureParams(uint32_t target, bool mipmaps);

    vec4 m_ambient;
    vec4 m_diffuse;
    vec4 m_specular;
    float m_shine;
    uint32_t m_texture;
};

#endif
