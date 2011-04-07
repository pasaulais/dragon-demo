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

////////////////////////////////////////////////////////////////////////////////

matrix4::matrix4()
{
    for(int i = 0; i < 16; i++)
        d[i] = 0.0;
}

vec3 matrix4::map(const vec3 &v)
{
    float v_w = 1.0;
    float x = d[0] * v.x + d[1] * v.y + d[2] * v.z + d[3] * v_w;
    float y = d[4] * v.x + d[5] * v.y + d[6] * v.z + d[7] * v_w;
    float z = d[8] * v.x + d[9] * v.y + d[10] * v.z + d[11] * v_w;
    float w = d[12] * v.x + d[13] * v.y + d[14] * v.z + d[15] * v_w;
    return vec3(x / w, y / w, z / w);
}

vec3 matrix4::mapNormal(const vec3 &v)
{
    float v_w = 1.0;
    float x = d[0] * v.x + d[1] * v.y + d[2] * v.z;
    float y = d[4] * v.x + d[5] * v.y + d[6] * v.z;
    float z = d[8] * v.x + d[9] * v.y + d[10] * v.z;
    float w = d[12] * v.x + d[13] * v.y + d[14] * v.z + v_w;
    return vec3(x / w, y / w, z / w);
}
