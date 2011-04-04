#include <cmath>
#include <GL/gl.h>
#include "Primitive.h"
#include "Mesh.h"

Mesh * Primitive::createCurve(double theta, double width, GLuint nCurvePoints)
{
    double theta_rad = radians(theta);
    Mesh *m = new Mesh();
    
    // compute the vertices of the curve
    curveVertices(m, theta_rad, width, nCurvePoints);
    
    // compute the normals of the curve
    curveNormals(m, theta_rad, width, nCurvePoints);
    
    // compute the texture coords of the curve
    curveTexcoords(m, theta_rad, width, nCurvePoints);
    
    // specify the model using triangle strips for the different faces
    curveMeshes(m, theta_rad, width, nCurvePoints);
    return m;
}

void Primitive::curveVertices(Mesh *m, double theta, double width, GLuint nCurvePoints)
{
    // face 0 is front, face 1 is back, 2 is start leg, 3 is end leg
    int verticeCount = ((nCurvePoints * 8) + 8);
    GLfloat *vertices = new GLfloat[3 * verticeCount];
    GLfloat *v0outer_up = vertices;
    GLfloat *v0inner_up = v0outer_up + (nCurvePoints * 3);
    GLfloat *v1outer_up = v0inner_up + (nCurvePoints * 3);
    GLfloat *v1inner_up = v1outer_up + (nCurvePoints * 3);
    GLfloat *v0outer_side = v1inner_up + (nCurvePoints * 3);
    GLfloat *v0inner_side = v0outer_side + (nCurvePoints * 3);
    GLfloat *v1outer_side = v0inner_side + (nCurvePoints * 3);
    GLfloat *v1inner_side = v1outer_side + (nCurvePoints * 3);
    GLfloat *v2 = v1inner_side + (nCurvePoints * 3);
    GLfloat *v3 = v2 + (4 * 3);
    
    double angle = 0.0;
    double step = theta / ((double)nCurvePoints - 1.0);
    double r = (1.0 - width);
    for(GLuint i = 0; i < nCurvePoints; i++, angle += step)
    {
        double x = cos(angle);
        double y = sin(angle);
        
        // outer curve
        *v0outer_up++ = *v0outer_side++ = x;
        *v0outer_up++ = *v0outer_side++ = y;
        *v0outer_up++ = *v0outer_side++ = -0.5;
        
        *v1outer_up++ = *v1outer_side++ = x;
        *v1outer_up++ = *v1outer_side++ = y;
        *v1outer_up++ = *v1outer_side++ = 0.5;
        
        // inner curve
        *v0inner_up++ = *v0inner_side++ = x * r;
        *v0inner_up++ = *v0inner_side++ = y * r;
        *v0inner_up++ = *v0inner_side++ = -0.5;
        
        *v1inner_up++ = *v1inner_side++ = x * r;
        *v1inner_up++ = *v1inner_side++ = y * r;
        *v1inner_up++ = *v1inner_side++ = 0.5;
    }
    
    // start face
    *v2++ =   r; *v2++ = 0.0; *v2++ = -0.5;
    *v2++ = 1.0; *v2++ = 0.0; *v2++ = -0.5;
    *v2++ = 1.0; *v2++ = 0.0; *v2++ =  0.5;
    *v2++ =   r; *v2++ = 0.0; *v2++ =  0.5;
    
    // end face
    *v3++ =   r * cos(theta); *v3++ =   r * sin(theta); *v3++ = -0.5;
    *v3++ = 1.0 * cos(theta); *v3++ = 1.0 * sin(theta); *v3++ = -0.5;
    *v3++ = 1.0 * cos(theta); *v3++ = 1.0 * sin(theta); *v3++ =  0.5;
    *v3++ =   r * cos(theta); *v3++ =   r * sin(theta); *v3++ =  0.5;

    m->setVertices(vertices, verticeCount);
    delete [] vertices;
}

void Primitive::curveNormals(Mesh *m, double theta, double width, GLuint nCurvePoints)
{   
    // face 0 is front, face 1 is back, 2 is start leg, 3 is end leg
    int normalCount = ((nCurvePoints * 8) + 8);
    GLfloat *normals = new GLfloat[3 * normalCount];
    GLfloat *n0outer_up = normals;
    GLfloat *n0inner_up = n0outer_up + (nCurvePoints * 3);
    GLfloat *n1outer_up = n0inner_up + (nCurvePoints * 3);
    GLfloat *n1inner_up = n1outer_up + (nCurvePoints * 3);
    GLfloat *n0outer_side = n1inner_up + (nCurvePoints * 3);
    GLfloat *n0inner_side = n0outer_side + (nCurvePoints * 3);
    GLfloat *n1outer_side = n0inner_side + (nCurvePoints * 3);
    GLfloat *n1inner_side = n1outer_side + (nCurvePoints * 3);
    GLfloat *n2 = n1inner_side + (nCurvePoints * 3);
    GLfloat *n3 = n2 + (4 * 3);
    
    double angle = 0.0;
    double step = theta / ((double)nCurvePoints - 1.0);
    double r = (1.0 - width);
    for(GLuint i = 0; i < nCurvePoints; i++, angle += step)
    {
        double x = cos(angle);
        double y = sin(angle);
        
        // outer curve
        *n0outer_up++ = x;      *n0outer_side++ =  0.0;
        *n0outer_up++ = y;      *n0outer_side++ =  0.0;
        *n0outer_up++ = 0.0;    *n0outer_side++ = -1.0;
        
        *n1outer_up++ = x;      *n1outer_side++ =  0.0;
        *n1outer_up++ = y;      *n1outer_side++ =  0.0;
        *n1outer_up++ = 0.0;    *n1outer_side++ =  1.0;
        
        // inner curve
        *n0inner_up++ = -x;     *n0inner_side++ =  0.0;
        *n0inner_up++ = -y;     *n0inner_side++ =  0.0;
        *n0inner_up++ = 0.0;    *n0inner_side++ = -1.0;
        
        *n1inner_up++ = -x;     *n1inner_side++ =  0.0;
        *n1inner_up++ = -y;     *n1inner_side++ =  0.0;
        *n1inner_up++ = 0.0;    *n1inner_side++ =  1.0;
    }
    
    // start face
    GLfloat dxs = (1.0 - r) * cos(0.0);
    GLfloat dys = (1.0 - r) * sin(0.0);
    *n2++ = dys; *n2++ = -dxs; *n2++ = 0.0;
    *n2++ = dys; *n2++ = -dxs; *n2++ = 0.0;
    *n2++ = dys; *n2++ = -dxs; *n2++ = 0.0;
    *n2++ = dys; *n2++ = -dxs; *n2++ = 0.0;
    
    // end face
    GLfloat dxe = (1.0 - r) * cos(theta);
    GLfloat dye = (1.0 - r) * sin(theta);
    *n3++ = -dye; *n3++ = dxe; *n3++ = 0.0;
    *n3++ = -dye; *n3++ = dxe; *n3++ = 0.0;
    *n3++ = -dye; *n3++ = dxe; *n3++ = 0.0;
    *n3++ = -dye; *n3++ = dxe; *n3++ = 0.0;

    m->setNormals(normals, normalCount);
    delete [] normals;
}

void Primitive::curveTexcoords(Mesh *m, double theta, double width, GLuint nCurvePoints)
{
    // face 0 is front, face 1 is back, 2 is start leg, 3 is end leg
    int texCoordsCount = ((nCurvePoints * 8) + 8);
    GLfloat *texCoords = new GLfloat[2 * texCoordsCount];
    GLfloat *t0outer_up = texCoords;
    GLfloat *t0inner_up = t0outer_up + (nCurvePoints * 2);
    GLfloat *t1outer_up = t0inner_up + (nCurvePoints * 2);
    GLfloat *t1inner_up = t1outer_up + (nCurvePoints * 2);
    GLfloat *t0outer_side = t1inner_up + (nCurvePoints * 2);
    GLfloat *t0inner_side = t0outer_side + (nCurvePoints * 2);
    GLfloat *t1outer_side = t0inner_side + (nCurvePoints * 2);
    GLfloat *t1inner_side = t1outer_side + (nCurvePoints * 2);
    GLfloat *t2 = t1inner_side + (nCurvePoints * 2);
    GLfloat *t3 = t2 + (4 * 2);
    
    double angle = 0.0;
    double step = theta / ((double)nCurvePoints - 1.0);
    double r = (1.0 - width);
    for(GLuint i = 0; i < nCurvePoints; i++, angle += step)
    {
        double tc = (double)i / (double)nCurvePoints; // normalized texture coord
        
        // outer curve
        *t0outer_up++ = tc;     *t0outer_side++ =  1.0;
        *t0outer_up++ = 1.0;    *t0outer_side++ =  tc;
        
        *t1outer_up++ = tc;     *t1outer_side++ =  0.0;
        *t1outer_up++ = 0.0;    *t1outer_side++ =  tc;
        
        // inner curve
        *t0inner_up++ = tc;     *t0inner_side++ =  0.0;
        *t0inner_up++ = 0.0;    *t0inner_side++ =  tc;
        
        *t1inner_up++ = tc;     *t1inner_side++ =  1.0;
        *t1inner_up++ = 1.0;    *t1inner_side++ =  tc;
    }
    
    // start face
    *t2++ = 1.0; *t2++ = 1.0;
    *t2++ = 1.0; *t2++ = 0.0;
    *t2++ = 0.0; *t2++ = 0.0;
    *t2++ = 0.0; *t2++ = 1.0;
    
    // end face
    *t3++ = 1.0; *t3++ = 1.0;
    *t3++ = 1.0; *t3++ = 0.0;
    *t3++ = 0.0; *t3++ = 0.0;
    *t3++ = 0.0; *t3++ = 1.0;

    m->setTexCoords(texCoords, texCoordsCount);
    delete [] texCoords;
}

void Primitive::curveMeshes(Mesh *m, double theta, double width, GLuint nCurvePoints)
{
    int offset[6];
    int vertices[6];
    for(GLuint i = 0; i < 4; i++)
    {
        vertices[i] = nCurvePoints * 2;
        offset[i] = i * nCurvePoints * 2;
    }
    vertices[4] = 4;
    vertices[5] = 4;
    offset[4] = (nCurvePoints * 2 * 4);
    offset[5] = offset[4] + vertices[4];

    int indiceCount = ((nCurvePoints * 2 * 4) + 8);
    GLuint *indices = new GLuint[indiceCount];
    GLuint *i0 = indices + offset[0];
    GLuint *i1 = indices + offset[1];
    GLuint *i2 = indices + offset[2];
    GLuint *i3 = indices + offset[3];
    GLuint *i4 = indices + offset[4];
    GLuint *i5 = indices + offset[5];
    for(GLuint i = 0; i < nCurvePoints; i++)
    {
        *i0++ = i + (nCurvePoints * 5);
        *i0++ = i + (nCurvePoints * 4);
        *i1++ = i + (nCurvePoints * 7);
        *i1++ = i + (nCurvePoints * 6);
        *i2++ = i + (nCurvePoints * 0);
        *i2++ = i + (nCurvePoints * 2);
        *i3++ = i + (nCurvePoints * 1);
        *i3++ = i + (nCurvePoints * 3);
    }
    for(GLuint i = 0; i < 4; i++)
    {
        i4[i] = offset[4] + i;
        i5[i] = offset[5] + i;
    }
    m->setIndices(indices, indiceCount);
    for(GLuint i = 0; i < 4; i++)
        m->addFace(GL_TRIANGLE_STRIP, vertices[i], offset[i]);
    for(GLuint i = 4; i < 6; i++)
        m->addFace(GL_QUADS, vertices[i], offset[i]);
    delete [] indices;
}

Mesh * Primitive::createCube()
{
    static GLfloat vertices[][3] =
    {
        {-1.0, -1.0,  1.0}, {-1.0,  1.0,  1.0},
        { 1.0,  1.0,  1.0}, { 1.0, -1.0,  1.0},
        {-1.0, -1.0, -1.0}, {-1.0,  1.0, -1.0},
        { 1.0,  1.0, -1.0}, { 1.0, -1.0, -1.0}
    };
#ifdef EXACT_NORMALS
    static GLfloat faces_normals[][3] =
    {
        { 0.0,  0.0,  1.0}, { 1.0,  0.0,  0.0},
        { 0.0, -1.0,  0.0}, { 0.0,  1.0,  0.0},
        { 0.0,  0.0, -1.0}, {-1.0,  0.0,  0.0},
    };
#else
    // let create_mesh() compute face normals
    GLfloat *faces_normals = 0;
#endif
    static GLuint faces_indices[][4] =
    {
        {0, 3, 2, 1}, {2, 3, 7, 6},
        {0, 4, 7, 3}, {1, 2, 6, 5},
        {4, 5, 6, 7}, {0, 1, 5, 4}
    };
    int indiceCount = 24, faceCount = 6, verticesPerFace = indiceCount / faceCount;
    Mesh *m = new Mesh();
    m->setVertices((GLfloat *)vertices, 8);
    m->setIndices((GLuint *)faces_indices, indiceCount);
    for(int i = 0; i < faceCount; i++)
        m->addFace(GL_QUADS, verticesPerFace, i * verticesPerFace);
    if(faces_normals)
        m->setNormals((GLfloat *)faces_normals, faceCount);
    else
        m->computeNormals(indiceCount);
    //m->generate_quadri_textcoords(indiceCount);
    return m;
}

Mesh * Primitive::createShearedParalpd(double width, double height, double theta)
{
    GLfloat d = height / tan(radians(theta));
    GLfloat vertices[8][3] =
    {
        {0.0, 0.0, 0.0},          {d, height, 0.0},
        {width + d, height, 0.0}, {width, 0.0, 0.0},
        {0.0, 0.0, 1.0},          {d, height, 1.0},
        {width + d, height, 1.0}, {width, 0.0, 1.0},
    };
#ifdef EXACT_NORMALS
    GLfloat a = sqrt(height * height + d * d);
    GLfloat faces_normals[6][3] =
    {
        { 0.0,  0.0, -1.0},     { 0.0,  0.0,  1.0},
        {-height/a, d/a,  0.0}, {height/a, -d/a,  0.0},
        { 0.0,  1.0,  0.0},     { 0.0, -1.0,  0.0},
    };
#else
    // let create_mesh() compute face normals
    GLfloat *faces_normals = 0;
#endif
    static GLuint faces_indices[][4] =
    {
        {0, 1, 2, 3}, {4, 7, 6, 5},
        {0, 4, 5, 1}, {3, 2, 6, 7},
        {1, 5, 6, 2}, {0, 3, 7, 4}
    };
    int indiceCount = 24, faceCount = 6, verticesPerFace = indiceCount / faceCount;
    Mesh *m = new Mesh();
    m->setVertices((GLfloat *)vertices, 8);
    m->setNormals((GLfloat *)faces_normals, faceCount);
    m->setIndices((GLuint *)faces_indices, indiceCount);
    for(int i = 0; i < faceCount; i++)
        m->addFace(GL_QUADS, verticesPerFace, i * verticesPerFace);
    //m->generate_quadri_textcoords(indiceCount);
    return m;
}

double Primitive::shearedParalpdWidth(double baseWidth, double height, double theta)
{
    GLfloat theta_rad = (theta / 180.0) * M_PI;
    GLfloat d = height / tan(theta_rad);
    return d + baseWidth;
}
