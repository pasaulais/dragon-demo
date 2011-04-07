#ifndef INITIALS_VERTEX_H
#define INITIALS_VERTEX_H

#include <QVector3D>

class vec2
{
public:
    float x;
    float y;

    inline vec2()
    {
    }

    inline vec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
};

class vec3
{
public:
    float x;
    float y;
    float z;

    inline vec3()
    {
    }

    inline vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    inline vec3(const QVector3D &v)
    {
        this->x = v.x();
        this->y = v.y();
        this->z = v.z();
    }

    operator QVector3D() const;

    static vec3 normal(const vec3 &a, const vec3 &b, const vec3 &c);
};

vec3 operator+(const vec3 &a, const vec3 &b);
vec3 operator-(const vec3 &a, const vec3 &b);

class vec4
{
public:
    float x;
    float y;
    float z;
    float w;

    inline vec4()
    {
    }

    inline vec4(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
};

#endif
