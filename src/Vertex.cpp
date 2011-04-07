#include <cmath>
#include "Vertex.h"

vec3 vec3::normal(const vec3 &a, const vec3 &b, const vec3 &c)
{
    // calculate the cross-product of AB and AC
    vec3 n, u = b - a, v = c - a;
    n.x = (u.y * v.z - u.z * v.y);
    n.y = (u.z * v.x - u.x * v.z);
    n.z = (u.x * v.y - u.y * v.x);

    // normalize it
    float w = (float)sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
    n.x = (n.x / w);
    n.y = (n.y / w);
    n.z = (n.z / w);
    return n;
}

vec3::operator QVector3D() const
{
    return QVector3D(x, y, z);
}

vec3 operator+(const vec3 &a, const vec3 &b)
{
    vec3 u;
    u.x = a.x + b.x;
    u.y = a.y + b.y;
    u.z = a.z + b.z;
    return u;
}

vec3 operator-(const vec3 &a, const vec3 &b)
{
    vec3 u;
    u.x = a.x - b.x;
    u.y = a.y - b.y;
    u.z = a.z - b.z;
    return u;
}
