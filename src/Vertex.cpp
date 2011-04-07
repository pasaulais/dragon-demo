#include <cmath>
#include "Vertex.h"

vec3 normalVector(const vec3 &a, const vec3 &b, const vec3 &c)
{
    // calculate the cross-product of AB and AC
    vec3 n, u = b - a, v = c - a;
    n.setX(u.y() * v.z() - u.z() * v.y());
    n.setY(u.z() * v.x() - u.x() * v.z());
    n.setZ(u.x() * v.y() - u.y() * v.x());

    // normalize it
    float w = (float)sqrt(n.x() * n.x() + n.y() * n.y() + n.z() * n.z());
    n.setX(n.x() / w);
    n.setY(n.y() / w);
    n.setZ(n.z() / w);
    return n;
}
