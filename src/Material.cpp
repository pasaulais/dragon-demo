#include <tiffio.h>
#include <cstring>
#include <cstdio>
#include "Platform.h"
#include "Material.h"

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

void setTextureParams(uint32_t target, bool mipmaps)
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
    m_texture = textureFromTIFFImage(path, mipmaps);
}

void Material::loadTextureTIFF(const char *data, size_t size, bool mipmaps)
{
    m_texture = textureFromTIFFImage(data, size, mipmaps);
}

static uint8_t check_texture[] =
{
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff
};

uint32_t textureFromTIFF(TIFF *tiff, bool mipmaps)
{
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
    uint32_t *data2 = (uint32_t *)check_texture;
    width = 4;
    height = 4;
#ifdef JNI_WRAPPER
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, data2);
    setTextureParams(GL_TEXTURE_2D, false);
#else
    if(mipmaps)
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height,
            GL_RGB, GL_UNSIGNED_BYTE, data2);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, data2);
    setTextureParams(GL_TEXTURE_2D, mipmaps);
#endif
    glBindTexture(GL_TEXTURE_2D, 0);
    _TIFFfree(data);
    return texID;
}

uint32_t Material::textureFromTIFFImage(string path, bool mipmaps)
{
    TIFF *tiff = TIFFOpen(path.c_str(), "r");
    if(!tiff)
        return 0;
    uint32_t texID = textureFromTIFF(tiff, mipmaps);
    TIFFClose(tiff);
    return texID;
}

typedef struct
{
    const char *buffer;
    size_t size;
    int32_t pos;
    bool closed;
} tiff_stream;

tsize_t tiff_Read(thandle_t st, tdata_t buffer, tsize_t size)
{
    tiff_stream *s = (tiff_stream *)st;
    if(s->closed)
        return 0;
    int32_t left = (s->size - s->pos);
    left = max(min(left, size), 0);
    memcpy(buffer, s->buffer + s->pos, left);
    s->pos += left;
    return left;
}

tsize_t tiff_Write(thandle_t st, tdata_t buffer, tsize_t size)
{
    (void)st;
    (void)buffer;
    (void)size;
    return 0;
}

int tiff_Close(thandle_t st)
{
    tiff_stream *s = (tiff_stream *)st;
    s->closed = true;
    return 0;
}

toff_t tiff_Seek(thandle_t st, toff_t pos, int whence)
{
    tiff_stream *s = (tiff_stream *)st;
    if(s->closed)
        return 0;
    switch(whence)
    {
    case SEEK_SET:
        s->pos = pos;
        break;
    case SEEK_CUR:
        s->pos = s->pos + pos;
        break;
    case SEEK_END:
        s->pos = s->size + pos - 1;
        break;
    }
    s->pos = min(max(s->pos, 0), (int32_t)(s->size - 1));
    return s->pos;
}

toff_t tiff_Size(thandle_t st)
{
    tiff_stream *s = (tiff_stream *)st;
    return s->size;
}

int tiff_Map(thandle_t, tdata_t*, toff_t*)
{
    return 0;
}

void tiff_Unmap(thandle_t, tdata_t, toff_t)
{
}

uint32_t Material::textureFromTIFFImage(const char *data, size_t size, bool mipmaps)
{
    tiff_stream s;
    s.buffer = data;
    s.size = size;
    s.pos = 0;
    s.closed = false;
    TIFF *tiff = TIFFClientOpen("<memory>", "r", (thandle_t)&s,
        tiff_Read, tiff_Write, tiff_Seek, tiff_Close, tiff_Size, tiff_Map, tiff_Unmap);
    if(!tiff)
        return 0;
    uint32_t texID = textureFromTIFF(tiff, mipmaps);
    TIFFClose(tiff);
    return texID;
}
