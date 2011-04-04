#include <GL/gl.h>
#include "Material.h"

Material::Material()
{
    m_ambient = QVector4D(0.0, 0.0, 0.0, 0.0);
    m_diffuse = QVector4D(0.0, 0.0, 0.0, 0.0);
    m_specular = QVector4D(0.0, 0.0, 0.0, 0.0);
    m_shine = 0.0;
    m_texture = 0;
    m_useMipmaps = false;
}

Material::Material(QVector4D ambient, QVector4D diffuse, QVector4D specular, float shine)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shine = shine;
    m_texture = 0;
    m_useMipmaps = false;
}

Material::Material(QVector4D ambient, QVector4D diffuse, QVector4D specular, float shine, bool useMipmaps)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shine = shine;
    m_texture = 0;
    m_useMipmaps = useMipmaps;
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
    if(m_useMipmaps)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Material::endApply()
{
    glPopAttrib();
}
