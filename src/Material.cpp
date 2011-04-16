#include <tiffio.h>
#include "Material.h"

#ifdef JNI_WRAPPER
#include <GLES/gl.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

Material::Material()
{
    m_ambient = vec4(0.0, 0.0, 0.0, 0.0);
    m_diffuse = vec4(0.0, 0.0, 0.0, 0.0);
    m_specular = vec4(0.0, 0.0, 0.0, 0.0);
    m_shine = 0.0;
    m_texture = 0;
}

Material::Material(vec4 ambient, vec4 diffuse, vec4 specular, float shine)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shine = shine;
    m_texture = 0;
}

const vec4 & Material::ambient() const
{
    return m_ambient;
}

const vec4 & Material::diffuse() const
{
    return m_diffuse;
}

const vec4 & Material::specular() const
{
    return m_specular;
}

float Material::shine() const
{
    return m_shine;
}

void Material::setAmbient(const vec4 &ambient)
{
    m_ambient = ambient;
}

void Material::setDiffuse(const vec4 &diffuse)
{
    m_diffuse = diffuse;
}

void Material::setSpecular(const vec4 &specular)
{
    m_specular = specular;
}

void Material::setShine(float shine)
{
    m_shine = shine;
}

uint32_t Material::texture() const
{
    return m_texture;
}

void Material::setTexture(uint32_t texture)
{
    m_texture = texture;
}

void Material::freeTexture()
{
    if(m_texture != 0)
    {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

void Material::setTextureParams(uint32_t target, bool mipmaps)
{
    if(mipmaps)
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Material::loadTextureTIFF(string path, bool mipmaps)
{
    TIFFOpen(path.c_str(), "r");
    m_texture = textureFromTIFFImage(path, mipmaps);
}

// Create a texture from a TIFF file using libtiff
uint32_t Material::textureFromTIFFImage(string path, bool mipmaps)
{
    // load the image
    TIFF *tiff = TIFFOpen(path.c_str(), "r");
    if(!tiff)
        return 0;

    uint32_t width, height;
    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);

    uint32_t *data = (uint32_t *) _TIFFmalloc(width * height * sizeof(uint32_t));
    if(!data)
    {
        TIFFClose(tiff);
        return 0;
    }

    if(!TIFFReadRGBAImage(tiff, width, height, data, 1))
    {
        _TIFFfree(data);
        TIFFClose(tiff);
        return 0;
    }

    // create a texture
    uint32_t texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
#ifdef JNI_WRAPPER
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);
    setTextureParams(GL_TEXTURE_2D, false);
#else
    if(mipmaps)
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
    setTextureParams(GL_TEXTURE_2D, mipmaps);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
    _TIFFfree(data);
    TIFFClose(tiff);
    return texID;
}
