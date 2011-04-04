#include <GL/gl.h>
#include <cmath>
#include <QVector3D>
#include "RobotArm.h"
#include "Mesh.h"

static Material metal_material(QVector4D(0.2, 0.2, 0.2, 1.0),
    QVector4D(0.4, 0.4, 0.4, 1.0), QVector4D(0.2, 0.2, 0.2, 1.0), 20.0);

static Material blue_material(QVector4D(0.0, 0.0, 0.2, 1.0),
    QVector4D(0.0, 0.0, 0.1, 1.0), QVector4D(0.0, 0.0, 0.2, 1.0), 20.0);

RobotJoint::RobotJoint()
{
    m_length = 1.0;
    m_theta = 0.0;
    m_axis =  QVector3D(0.0, 1.0, 0.0);
    m_orientation = QVector4D();
}

void RobotJoint::setJointParameters(JointID id)
{
    m_theta = 0.0;
    m_orientation = QVector4D();
    m_axis = QVector3D(1.0, 0.0, 0.0);
    switch(id)
    {
    case AX12_BASE:
        m_orientation = QVector4D(0.0, 0.0, 1.0, 090.0);
        m_length = 0.850;
        m_axis = QVector3D(0.0, 0.0, 1.0);
        break;
    case AX12_SEGMENT_A:
        m_orientation = QVector4D(1.0, 0.0, 0.0, 180.0);
        m_length = 0.770;
        break;
    case AX12_SEGMENT_B:
        m_orientation = QVector4D(1.0, 0.0, 0.0, 180.0);
        m_length = 1.746;
        break;
    case AX12_SEGMENT_C:
        m_length = 1.230;
        break;
    case AX12_SEGMENT_D:
        m_length = 0.75;
        break;
    case AX12_SEGMENT_E:
        m_length = 1.11;
        break;
    }
}

float RobotJoint::theta() const
{
    return m_theta;
}

void RobotJoint::setTheta(float newTheta)
{
    m_theta = newTheta;
}

float RobotJoint::length() const
{
    return m_length;
}

void RobotJoint::setLength(float newLen)
{
    m_length = newLen;
}

QVector4D RobotJoint::orientation() const
{
    return m_orientation;
}

void RobotJoint::setOrientation(const QVector4D &newOri)
{
    m_orientation = newOri;
}

QVector3D RobotJoint::axis() const
{
    return m_axis;
}

void RobotJoint::setAxis(const QVector3D &newAxis)
{
    m_axis = newAxis;
}

////////////////////////////////////////////////////////////////////////////////

RobotArm::RobotArm(QObject *parent) : QObject(parent)
{
    m_loadedMeshes = 0;
    m_missingMeshes = 0;
    m_cursor = 0;
    m_joints.resize(6);
}

RobotArm::~RobotArm()
{
}

void RobotArm::reset()
{
    m_joints[0].setJointParameters(RobotJoint::AX12_BASE);
    m_joints[1].setJointParameters(RobotJoint::AX12_SEGMENT_A);
    m_joints[2].setJointParameters(RobotJoint::AX12_SEGMENT_B);
    m_joints[3].setJointParameters(RobotJoint::AX12_SEGMENT_C);
    m_joints[4].setJointParameters(RobotJoint::AX12_SEGMENT_D);
    m_joints[5].setJointParameters(RobotJoint::AX12_SEGMENT_E);
    for(int i = 0; i < jointCount(); i++)
        emit jointAngleChanged(i, jointAngle(i));
    m_drawSkeleton = false;
}

bool RobotArm::load()
{
    if(meshLoaded())
        return true;
    reset();
    m_joint = loadMesh("meshes/joint.stl");
    m_base = loadMesh("meshes/base.stl");
    m_turntable = loadMesh("meshes/turntable.stl");
    m_longJoint = loadMesh("meshes/long.stl");
    m_smallJoint = loadMesh("meshes/small.stl");
    m_bigJoint = loadMesh("meshes/big.stl");
    m_servo = loadMesh("meshes/servo.stl");
    m_link = loadMesh("meshes/link.stl");
    m_bracket = loadMesh("meshes/bracket.stl");
    m_holder = loadMesh("meshes/holder.stl");
    m_side = loadMesh("meshes/side.stl");
    return meshLoaded();
}

Mesh * RobotArm::loadMesh(QString path)
{
    Mesh *m = Mesh::load_stl(path.toUtf8().constData(), this);
    if(m)
        m_loadedMeshes++;
    else
        m_missingMeshes++;
    return m;
}

bool RobotArm::meshLoaded() const
{
    return m_loadedMeshes > 0;
}

int RobotArm::jointCount() const
{
    return m_joints.count();
}

float RobotArm::jointAngle(int index) const
{
    if(index >= 0 && index < jointCount())
        return m_joints[index].theta();
    else
        return 0.0;
}

void RobotArm::setJointAngle(int index, float newAngle)
{
    if(index < 0 || index >= jointCount())
        return;
    else if(newAngle != newAngle) // NaN
        return;
    float oldAngle = jointAngle(index);
    if(!fequal(oldAngle, newAngle))
    {
        m_joints[index].setTheta(newAngle);
        emit jointAngleChanged(index, newAngle);
    }
}

void RobotArm::moveTo(QVector3D pos, QVector3D orientation)
{
    QVector<double> angles = ikin(pos, orientation.z());
    if(angles.count() >= 6)
    {
        for(int i = 0; i < 6; i++)
            setJointAngle(i, angles[i]);
    }
}

QVector<double> RobotArm::ikin(QVector3D eepos, double xyAngle)
{
    const double theta2 = M_PI/4;
    double pos[6][3]; //position of each joint
    QVector<double> lengths;
    QVector<double> newThetas(6); //new angles
    double phi = (xyAngle / 180.0) * M_PI;
    double len;
    double s1,c1;
    double temp1;
    double d35,ca,sa,a,d25,cb,sb,b;
    double sg,cg,g;
    for(int i = 0; i < 6; i++)
        lengths.append(m_joints[i].length());

    //theta2 is fixed
    newThetas[1] = theta2;

    pos[0][0] = 0; pos[0][1] = 0; pos[0][2] = 0;

    pos[1][0] = 0; pos[1][1] = 0; pos[1][2] = lengths[0];

    pos[5][0] = eepos.x() - lengths[5] * cos(phi);
    pos[5][1] = eepos.y() - lengths[5] * sin(phi);
    pos[5][2] = eepos.z();

    len = sqrt( pow(pos[5][0],2) + pow(pos[5][1],2) );
    s1 = pos[5][1] / len;
    c1 = pos[5][0] / len;
    newThetas[0] = atan2(s1,c1);
    newThetas[5] = phi - newThetas[0];

    pos[4][0] = pos[5][0] - lengths[4] * c1;
    pos[4][1] = pos[5][1] - lengths[4] * s1;
    pos[4][2] = pos[5][2];

    pos[2][0] = sin(theta2) * lengths[1] * c1;
    pos[2][1] = sin(theta2) * lengths[1] * s1;
    pos[2][2] = pos[1][2] + cos(theta2) * lengths[1];

    temp1 = pow((pos[2][0]-pos[4][0]),2) + pow((pos[2][1]-pos[4][1]),2) + pow((pos[2][2]-pos[4][2]),2);
    if(temp1 < 0)
        return QVector<double>();
    d35 = sqrt(temp1);
    ca = (pow(d35,2) + pow(lengths[2],2) - pow(lengths[3],2)) / (2 * lengths[2] * d35);
    temp1 = 1 - pow(ca,2);
    if(temp1 < 0)
        return QVector<double>();
    sa = sqrt(temp1);
    a = atan2(sa,ca);
    temp1 = pow((pos[1][0]-pos[4][0]),2) + pow((pos[1][1]-pos[4][1]),2) + pow((pos[1][2]-pos[4][2]),2);
    if(temp1 < 0)
        return QVector<double>();
    d25 = sqrt(temp1);
    cb = (pow(d35,2) + pow(lengths[1],2) - pow(d25,2))/(2 * lengths[1] * d35);
    temp1 = 1 - pow(cb,2);
    if(temp1 < 0)
        return QVector<double>();
    sb = sqrt(temp1);
    b = atan2(sb,cb);
    newThetas[2] = M_PI - a - b;

    cg = (pow(lengths[2],2) + pow(lengths[3],2) - pow(d35,2)) / (2 * lengths[2] * lengths[3]);
    temp1 = 1 - pow(cg,2);
    if(temp1 < 0)
        return QVector<double>();
    sg = sqrt(temp1);
    g = atan2(sg,cg);
    newThetas[3] = M_PI - g;

    newThetas[4] = M_PI_2 - newThetas[3] - newThetas[2] - newThetas[1];

    // normalise angles and convert to degrees
    for(int i = 0; i < 6; i++)
        newThetas[i] = fmod(newThetas[i], 2.0 * M_PI) * (180.0 / M_PI);
    return newThetas;
}

void RobotArm::draw()
{
    if(!meshLoaded())
        return;
    glMatrixMode(GL_MODELVIEW);
    metal_material.beginApply();
    drawAx12Base();
    metal_material.endApply();
    if(m_drawSkeleton)
        drawSkeleton();
}

void RobotArm::transformJoint(int i)
{
    RobotJoint &j = m_joints[i];
    transformJoint(j.theta(), j.axis(), j.orientation());
}

void RobotArm::transformJoint(float theta, QVector3D axis, QVector4D ori)
{
    glRotatef(theta, axis.x(), axis.y(), axis.z());
    glRotatef(ori.w(), ori.x(), ori.y(), ori.z());
}

void RobotArm::drawSkeleton()
{
    static QVector3D axes[6] = {
        QVector3D(0.0, 0.0, 1.0), QVector3D(0.0, 1.0, 0.0), QVector3D(0.0, 1.0, 0.0),
        QVector3D(0.0, 1.0, 0.0), QVector3D(0.0, 1.0, 0.0), QVector3D(0.0, 0.0, 1.0)
    };
    static QVector4D orientations[6] = {
        QVector4D(0.0, 1.0, 0.0, -90.0), QVector4D(0, 0, 0, 0), QVector4D(0, 0, 0, 0),
        QVector4D(0.0, 0.0, 0.0, 0.0), QVector4D(0, 0, 0, 0), QVector4D(0, 0, 0, 0)
    };

    glPushMatrix();
    blue_material.beginApply();
    for(int i = 0; i < 6; i++)
    {
        RobotJoint j = m_joints[i];
        transformJoint(j.theta(), axes[i], orientations[i]);
        glPushMatrix();
            glScalef(j.length(), 1.0, 1.0);
            m_joint->draw();
        glPopMatrix();
        glTranslatef(j.length(), 0.0, 0.0);
    }
    blue_material.endApply();
    glPopMatrix();
}

void RobotArm::drawMesh(Mesh *m)
{
    if(!m)
        return;
    m->draw();
}

void RobotArm::drawServo()
{
    static Material mat(
        QVector4D(0.1, 0.1, 0.1, 1.0),
        QVector4D(0.1, 0.1, 0.1, 1.0),
        QVector4D(0.0, 0.0, 0.0, 1.0), 0.0);
    mat.beginApply();
    drawMesh(m_servo);
    mat.endApply();
}

void RobotArm::drawAx12Base()
{
    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.350);
        drawMesh(m_base);
        transformJoint(0);
        drawAx12A();
    glPopMatrix();
}

void RobotArm::drawAx12A()
{
    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.500);
        glPushMatrix();
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            drawMesh(m_turntable);
        glPopMatrix();
        glPushMatrix();
            transformJoint(1);
            drawMesh(m_longJoint);
            glPushMatrix();
                glTranslatef(-0.280, 0.0, -0.775);
                glRotatef(-90.0, 1.0, 0.0, 0.0);
                drawServo();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.280, 0.0, -0.775);
                glRotatef(-90.0, 1.0, 0.0, 0.0);
                drawServo();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.0, 0.0, -0.775);
                transformJoint(2);
                drawAx12B();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void RobotArm::drawAx12B()
{
    glPushMatrix();
        glTranslatef(0.280, 0.0, 0.0);
        glRotatef(180.0, 1.0, 0.0, 0.0);
        drawMesh(m_smallJoint);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.280, 0.0, 0.0);
        glRotatef(180.0, 1.0, 0.0, 0.0);
        drawMesh(m_smallJoint);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.280, 0.0, 0.825);
        glRotatef(-90.0, 0.0, 0.0, 1.0);
        glRotatef(90.0, 0.0, 1.0, 0.0);
        drawMesh(m_link);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.280, 0.0, 0.825);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        glRotatef(90.0, 0.0, 1.0, 0.0);
        drawMesh(m_link);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.280, 0.0, 1.740);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        drawServo();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.280, 0.0, 1.740);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        drawServo();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0, 0.0, 1.740);
        transformJoint(3);
        drawAx12C();
    glPopMatrix();
}

void RobotArm::drawAx12C()
{
    glPushMatrix();
        glTranslatef(0.280, 0.0, 0.0);
        glRotatef(180.0, 1.0, 0.0, 0.0);
        drawMesh(m_bigJoint);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.280, 0.0, 0.0);
        glRotatef(180.0, 1.0, 0.0, 0.0);
        drawMesh(m_bigJoint);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.280, 0.0, 1.235);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        drawServo();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.280, 0.0, 1.235);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        drawServo();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0, 0.0, 1.235);
        transformJoint(4);
        drawAx12D();
    glPopMatrix();
}

void RobotArm::drawAx12D()
{
    glPushMatrix();
        glTranslatef(0.280, 0.0, 0.0);
        glRotatef(180.0, 1.0, 0.0, 0.0);
        drawMesh(m_smallJoint);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.280, 0.0, 0.0);
        glRotatef(180.0, 1.0, 0.0, 0.0);
        drawMesh(m_smallJoint);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.260, 0.0, 0.30);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        drawMesh(m_bracket);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-0.260, 0.0, 0.30);
        glRotatef(-90.0, 0.0, 0.0, 1.0);
        drawMesh(m_bracket);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0, 0.0, 0.75);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        glPushMatrix();
            glRotatef(90.0, 1.0, 0.0, 0.0);
            drawServo();
        glPopMatrix();
        glPushMatrix();
            transformJoint(5);
            drawAx12E();
        glPopMatrix();
    glPopMatrix();
}

void RobotArm::drawAx12E()
{
    glPushMatrix();
        glRotatef(180.0, 1.0, 0.0, 0.0);
        drawMesh(m_smallJoint);
    glPopMatrix();
    glPushMatrix();
        glRotatef(90.0, 0.0, 0.0, 1.0);
        glPushMatrix();
            glTranslatef(0.0, 0.0, 0.295);
            drawMesh(m_holder);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-0.173, -0.205, 1.064);
            glRotatef(-180.0, 0.0, 0.0, 1.0);
            drawMesh(m_side);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0.173, 0.215, 1.064);
            drawMesh(m_side);
        glPopMatrix();
    glPopMatrix();
}
