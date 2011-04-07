#ifndef INITIALS_VERTEX_H
#define INITIALS_VERTEX_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

vec3 normalVector(const vec3 &a, const vec3 &b, const vec3 &c);

#endif
