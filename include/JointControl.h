#ifndef ROBOTSIM_JOINT_CONTROL_H
#define ROBOTSIM_JOINT_CONTROL_H

#include <QWidget>

class QSlider;
class QSpinBox;

class JointControl : public QWidget
{
    Q_OBJECT
    
public:
    JointControl(int value = 0, int id = 0, QWidget *parent = 0);
    
    int value() const;

public slots:
    void setValue(int v);

signals:
    void valueChanged(int id, float value);

private:
    int m_id;
    int m_value;
    QSlider *m_slider;
    QSpinBox *m_spinbox;
};

#endif
