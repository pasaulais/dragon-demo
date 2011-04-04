#ifndef INITIALS_PRIMITIVES_H
#define INITIALS_PRIMITIVES_H

#include <QtGlobal>

class QObject;
class Mesh;

class Primitive
{
public:
    static Mesh * createCurve(double theta, double width, uint nCurvePoints, QObject *parent = 0);
    static Mesh * createCube(QObject *parent = 0);
    static Mesh * createShearedParalpd(double baseWidth, double height, double theta, QObject *parent = 0);
    static double shearedParalpdWidth(double baseWidth, double height, double theta);

private:
    static Mesh * fromFaces(GLfloat *facesVertices, uint *facesIndices,
                            uint indiceCount, uint faceCount, QObject *parent = 0);
    static void curveVertices(Mesh *m, double theta, double width, uint nCurvePoints);
    static void curveNormals(Mesh *m, double theta, double width, uint nCurvePoints);
    static void curveTexcoords(Mesh *m, double theta, uint nCurvePoints);
    static void curveMeshes(Mesh *m, uint nCurvePoints);
};

#endif
