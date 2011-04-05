#include <GL/gl.h>
#include <GL/glu.h>
#include <tiffio.h>
#include "Material.h"

Material::Material()
{
    m_ambient = QVector4D(0.0, 0.0, 0.0, 0.0);
    m_diffuse = QVector4D(0.0, 0.0, 0.0, 0.0);
    m_specular = QVector4D(0.0, 0.0, 0.0, 0.0);
    m_shine = 0.0;
    m_texture = 0;
}

Material::Material(QVector4D ambient, QVector4D diffuse, QVector4D specular, float shine)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shine = shine;
    m_texture = 0;
}

void Material::setAmbient(const QVector4D &ambient)
{
    m_ambient = ambient;
}

void Material::setTexture(uint texture)
{
    m_texture = texture;
}

void Material::beginApply()
{
    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat *)&m_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat *)&m_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat *)&m_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, m_shine);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Material::setTextureParams(GLenum target, bool mipmaps)
{
    if(mipmaps)
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Material::endApply()
{
    glPopAttrib();
}

void Material::loadTextureTIFF(QString path, bool mipmaps)
{
    m_texture = textureFromTIFFImage(path, mipmaps);
}

// Create a texture from a TIFF file using libtiff
uint Material::textureFromTIFFImage(QString path, bool mipmaps)
{
    // load the image
    TIFF *tiff = TIFFOpen(path.toLatin1().data(), "r");
    if(!tiff)
        return 0;

    uint width, height;
    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);

    uint *data = (uint *) _TIFFmalloc(width * height * sizeof(uint));
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
    GLuint texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    if(mipmaps)
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
    setTextureParams(GL_TEXTURE_2D, mipmaps);
    glBindTexture(GL_TEXTURE_2D, 0);
    _TIFFfree(data);
    TIFFClose(tiff);
    return texID;
}
