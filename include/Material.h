#ifndef INITIALS_MATERIAL_H
#define INITIALS_MATERIAL_H

#include <QVector4D>

class Material
{
public:
    Material();
    Material(QVector4D ambient, QVector4D diffuse, QVector4D specular, float shine);
    Material(QVector4D ambient, QVector4D diffuse, QVector4D specular, float shine, bool useMipmaps);
    void beginApply();
    void endApply();

    void setAmbient(const QVector4D &ambient);
    void setTexture(uint texture);

private:
    QVector4D m_ambient;
    QVector4D m_diffuse;
    QVector4D m_specular;
    float m_shine;
    uint m_texture;
    bool m_useMipmaps;
};

#endif
