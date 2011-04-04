#include <QSlider>
#include <QSpinBox>
#include <QHBoxLayout>
#include "JointControl.h"

JointControl::JointControl(int value, int id, QWidget *parent) : QWidget(parent)
{
    m_id = id;
    m_value = value;
    m_slider = new QSlider;
    m_slider->setMinimum(-359);
    m_slider->setMaximum(359);
    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setValue(value);
    m_spinbox = new QSpinBox;
    m_spinbox->setMinimum(-359);
    m_spinbox->setMaximum(359);
    m_spinbox->setSingleStep(5);
    m_spinbox->setValue(value);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_slider);
    layout->addWidget(m_spinbox);
    connect(m_spinbox, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
    connect(m_slider, SIGNAL(sliderMoved(int)), this, SLOT(setValue(int)));
}

int JointControl::value() const
{
    return m_value;
}

void JointControl::setValue(int v)
{
    if(m_value != v)
    {
        m_value = v;
        m_slider->setValue(v);
        m_spinbox->setValue(v);
        emit valueChanged(m_id, (float)v);
    }
}
