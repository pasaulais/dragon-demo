#ifndef INITIALS_MATERIAL_H
#define INITIALS_MATERIAL_H

#include <QString>
#include <QVector4D>

class Material
{
public:
    Material();
    Material(QVector4D ambient, QVector4D diffuse, QVector4D specular, float shine);
    void beginApply();
    void endApply();

    void setAmbient(const QVector4D &ambient);
    void setTexture(uint texture);
    void freeTexture();

    void loadTextureTIFF(QString path, bool mipmaps = false);

private:
    // Create a texture from a TIFF file using libtiff
    static uint textureFromTIFFImage(QString path, bool mipmaps);
    static void setTextureParams(GLenum target, bool mipmaps);

    QVector4D m_ambient;
    QVector4D m_diffuse;
    QVector4D m_specular;
    float m_shine;
    uint m_texture;
};

#endif
