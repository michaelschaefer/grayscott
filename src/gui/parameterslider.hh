#ifndef __PARAMETERSLIDER_HH__
#define __PARAMETERSLIDER_HH__


#include <QLabel>
#include <QSlider>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QtCore/Qt>


class ParameterSlider : public QWidget {

    Q_OBJECT


signals:

    void valueChanged();


public:

    ParameterSlider(QWidget* parent=0, QString name=QString())
        : QWidget(parent)
    {

        m_name = name;
        setFormat("%.3f");

        // create widgets
        m_label = new QLabel();
        m_slider = new QSlider(Qt::Orientation::Horizontal);
        QObject::connect(m_slider, SIGNAL(valueChanged(int)),
                         this, SLOT(update_label(int)));
        setRange(0, 100);
        setValue(50);

        // create layout
        QVBoxLayout* box = new QVBoxLayout();
        box->addWidget(m_label);
        box->addWidget(m_slider);
        setLayout(box);
    }

    void setFormat(QString format) {
        if (m_name.isNull() == true) {
            m_format = format;
        } else {
            m_format = QString("%s = ") + format;
        }
    }

    void setRange(double min, double max, double step=1) {
        double value = this->value();

        m_offset = min;
        m_delta = max - min;
        m_ticks = int(m_delta / step);

        m_slider->setMinimum(0);
        m_slider->setMaximum(m_ticks);
        m_slider->setSingleStep(1);
        m_slider->setTickInterval(1);
        m_slider->setPageStep(1);

        setValue(value);
    }

    inline void setValue(double value) {
        m_slider->setValue(extern_to_intern(value));
    }

    inline double value() const {
        return intern_to_extern(m_slider->value());
    }


private slots:

    void update_label(int value) {
        double internal_value = intern_to_extern(value);

        if (m_name.isNull() == true) {
            m_label->setText(QString().sprintf(m_format.toLocal8Bit().data() , internal_value));
        } else {
            m_label->setText(QString().sprintf(m_format.toLocal8Bit().data(), m_name.toLocal8Bit().data(), internal_value));
        }

        emit valueChanged();
    }


private:

    inline int extern_to_intern(double value) const {
        int v = (value - m_offset) * m_ticks / m_delta;
        if (v > m_ticks) {
            return m_ticks;
        } else {
            return v;
        }
    }

    inline double intern_to_extern(int value) const {
        return m_offset + m_delta * value / m_ticks;
    }


    int m_ticks;

    double m_delta;
    double m_offset;

    QLabel* m_label;

    QSlider* m_slider;

    QString m_format;
    QString m_name;

};


#endif // __PARAMETERSLIDER_HH__
