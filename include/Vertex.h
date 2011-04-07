#ifndef INITIALS_VERTEX_H
#define INITIALS_VERTEX_H

bool fequal(double a, double b);

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

class matrix4
{
public:
    float d[16];

    matrix4();

    vec3 map(const vec3 &v) const;
    vec3 mapNormal(const vec3 &v) const;

    void clear();
    void setIdentity();

    void dump() const;

    static matrix4 translate(float dx, float dy, float dz);
    static matrix4 rotate(float angle, float rx, float ry, float rz);
    static matrix4 scale(float sx, float sy, float sz);
};

matrix4 operator*(const matrix4 &a, const matrix4 &b);

#endif
