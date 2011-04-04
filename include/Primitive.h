#ifndef INITIALS_PRIMITIVES_H
#define INITIALS_PRIMITIVES_H

#include <GL/glut.h>

class Mesh;

#define M_PI		3.14159265358979323846	/* pi */
#define radians(t)  (((t) / 180.0) * M_PI)

class Primitive
{
public:
    static Mesh * createCurve(double theta, double width, GLuint nCurvePoints);
    static Mesh * createCube();
    static Mesh * createShearedParalpd(double baseWidth, double height, double theta);
    static double shearedParalpdWidth(double baseWidth, double height, double theta);

private:
    static Mesh * fromFaces(GLfloat *facesVertices, GLfloat *facesNormals,
                            GLuint *facesIndices, uint indiceCount, uint faceCount);
    static void curveVertices(Mesh *m, double theta, double width, GLuint nCurvePoints);
    static void curveNormals(Mesh *m, double theta, double width, GLuint nCurvePoints);
    static void curveTexcoords(Mesh *m, double theta, double width, GLuint nCurvePoints);
    static void curveMeshes(Mesh *m, double theta, double width, GLuint nCurvePoints);
};

#endif
