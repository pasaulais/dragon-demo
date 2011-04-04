#ifndef INITIALS_PRIMITIVES_H
#define INITIALS_PRIMITIVES_H

#include <QtGlobal>

class Mesh;

class Primitive
{
public:
    static Mesh * createCurve(double theta, double width, uint nCurvePoints);
    static Mesh * createCube();
    static Mesh * createShearedParalpd(double baseWidth, double height, double theta);
    static double shearedParalpdWidth(double baseWidth, double height, double theta);

private:
    static Mesh * fromFaces(GLfloat *facesVertices, uint *facesIndices, uint indiceCount, uint faceCount);
    static void curveVertices(Mesh *m, double theta, double width, uint nCurvePoints);
    static void curveNormals(Mesh *m, double theta, double width, uint nCurvePoints);
    static void curveTexcoords(Mesh *m, double theta, uint nCurvePoints);
    static void curveMeshes(Mesh *m, uint nCurvePoints);
};

#endif
