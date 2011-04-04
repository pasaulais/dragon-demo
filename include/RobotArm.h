#ifndef ROBOTSIM_ROBOT_ARM_H
#define ROBOTSIM_ROBOT_ARM_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QVector4D>

class Mesh;

class RobotJoint
{
public:
    enum JointID
    {
        AX12_BASE,
        AX12_SEGMENT_A,
        AX12_SEGMENT_B,
        AX12_SEGMENT_C,
        AX12_SEGMENT_D,
        AX12_SEGMENT_E,
    };

    RobotJoint();
    void setJointParameters(JointID id);

    float theta() const;
    void setTheta(float newTheta);
    float length() const;
    void setLength(float newLen);
    QVector4D orientation() const;
    void setOrientation(const QVector4D &newOri);
    QVector3D axis() const;
    void setAxis(const QVector3D &newAxis);

private:
    float m_length;
    // initial rotation
    QVector4D m_orientation;
    // axis of rotation
    QVector3D m_axis;
    // angle of rotation
    float m_theta;
};

class RobotArm : public QObject
{
    Q_OBJECT

public:
    RobotArm(QObject *parent = 0);
    virtual ~RobotArm();

    virtual bool load();
    bool meshLoaded() const;

    int jointCount() const;
    float jointAngle(int index) const;
    void setJointAngle(int index, float angle);

    virtual void draw();
    virtual void reset();
    void moveTo(QVector3D pos, QVector3D orientation);

signals:
    void jointAngleChanged(int jointIndex, float newAngle);

private:
    Mesh * loadMesh(QString path);
    void drawMesh(Mesh *m);
    void drawAx12Base();
    void drawAx12A();
    void drawAx12B();
    void drawAx12C();
    void drawAx12D();
    void drawAx12E();
    void drawServo();
    void drawSkeleton();
    void transformJoint(int index);
    void transformJoint(float theta, QVector3D axis, QVector4D orientation);
    QVector<double> ikin(QVector3D eePos, double xyAngle);

    Mesh *m_joint;
    Mesh *m_base;
    Mesh *m_turntable;
    Mesh *m_longJoint;
    Mesh *m_smallJoint;
    Mesh *m_bigJoint;
    Mesh *m_servo;
    Mesh *m_link;
    Mesh *m_bracket;
    Mesh *m_holder;
    Mesh *m_side;
    Mesh *m_cursor;
    int m_loadedMeshes, m_missingMeshes;
    QVector<RobotJoint> m_joints;
    bool m_drawSkeleton;
};

#endif
