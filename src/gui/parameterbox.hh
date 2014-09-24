#ifndef __PARAMETERBOX_HH__
#define __PARAMETERBOX_HH__


#include <QMap>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>
#include "src/gui/exportwidget.hh"
#include "src/gui/parameterslider.hh"
#include "src/gui/simulationviewer.hh"
#include "src/gui/text.hh"
#include "src/sim/parameters.hh"


using namespace std;


class ParameterBox : public QWidget {

    Q_OBJECT

public:

    ParameterBox(QWidget* parent=0)
        : QWidget(parent)
    {
        /*
         * Parameter sliders
         */

        ParameterSlider* slider = new ParameterSlider(this, "F");
        slider->setFormat("%.4f");
        slider->setRange(0, 0.1, 0.0001);
        slider->setValue(0.035);
        m_sliders["F"] = slider;

        slider = new ParameterSlider(this, "k");
        slider->setFormat("%.4f");
        slider->setRange(0, 0.1, 0.0001);
        slider->setValue(0.06);
        m_sliders["k"] = slider;

        slider = new ParameterSlider(this, Text::ParameterBox::System::size());
        slider->setFormat("%.0f");
        slider->setRange(32, 512, 32);
        slider->setValue(128);
        m_sliders["size"] = slider;


        /*
         * speedup group box
         */

        m_speedup = new QButtonGroup(this);
        QGroupBox* speedupGroupBox = new QGroupBox(Text::ParameterBox::Rendering::groupSpeedTitle());
        QHBoxLayout* speedupBoxLayout = new QHBoxLayout();

        QRadioButton* rb = new QRadioButton(Text::ParameterBox::Rendering::slow());
        m_speedup->addButton(rb, 100);
        speedupBoxLayout->addWidget(rb);

        rb = new QRadioButton(Text::ParameterBox::Rendering::normal());
        rb->setChecked(true);
        m_speedup->addButton(rb, 10);
        speedupBoxLayout->addWidget(rb);

        rb = new QRadioButton(Text::ParameterBox::Rendering::fast());
        m_speedup->addButton(rb, 1);
        speedupBoxLayout->addWidget(rb);

        rb = new QRadioButton(Text::ParameterBox::Rendering::veryFast());
        m_speedup->addButton(rb, 0);
        speedupBoxLayout->addWidget(rb);

        speedupGroupBox->setLayout(speedupBoxLayout);


        /*
         * Combo box for default coefficient settings
         */

        m_defaults = new QComboBox(this);
        m_defaults->setEditable(false);
        m_defaults->addItem(Text::ParameterBox::System::userDefined());
        m_defaults->addItem(Text::ParameterBox::System::bacteria1());
        m_defaults->addItem(Text::ParameterBox::System::unstable());
        m_defaults->addItem(Text::ParameterBox::System::zebra());

        QObject::connect(m_defaults, SIGNAL(activated(int)),
                         this, SLOT(load_defaults(int)));
        QObject::connect(m_defaults, SIGNAL(currentIndexChanged(int)),
                         this, SLOT(load_defaults(int)));
        QObject::connect(m_sliders["F"], SIGNAL(valueChanged()),
                this, SLOT(load_defaults()));
        QObject::connect(m_sliders["k"], SIGNAL(valueChanged()),
                this, SLOT(load_defaults()));        

        m_defaults->setCurrentIndex(1);                       


        /*
         * Export checkbox and path finder
         */

        m_export  = new ExportWidget(this);
        m_export->setChecked(false);


        /*
         * create and fill layout
         */

        QVBoxLayout* mainBox = new QVBoxLayout();

        QGroupBox* groupBox = new QGroupBox(Text::ParameterBox::System::groupTitle());
        QVBoxLayout* box = new QVBoxLayout();
        box->addWidget(m_defaults);
        box->addWidget(m_sliders["F"]);
        box->addWidget(m_sliders["k"]);
        box->addWidget(m_sliders["size"]);
        groupBox->setLayout(box);
        mainBox->addWidget(groupBox);

        groupBox = new QGroupBox(Text::ParameterBox::Rendering::groupTitle());
        box = new QVBoxLayout();        
        box->addWidget(speedupGroupBox);
        box->addWidget(m_export);

        groupBox->setLayout(box);
        mainBox->addWidget(groupBox);

        setLayout(mainBox);
    }    

    void loadParameters() {
        m_sliders["F"]->setValue(Parameters::Model::F);
        m_sliders["k"]->setValue(Parameters::Model::k);
        m_sliders["size"]->setValue(Parameters::Model::size);
        m_speedup->button(Parameters::Model::speedup)->setChecked(true);
        m_export->loadParameters();
    }

    bool saveParameters(SimulationViewer* simViewer=0) {
        if (m_export->saveParameters() == false) {
            return false;
        }

        Parameters::Model::F = m_sliders["F"]->value();
        Parameters::Model::k = m_sliders["k"]->value();
        Parameters::Model::size = m_sliders["size"]->value();
        Parameters::Model::speedup = m_speedup->checkedId();

        if (simViewer != 0) {
            if (m_export->isChecked() == true) {
                QString path = m_export->path();
                QString format = m_export->format();
                if (path.isNull() == true || format.isNull() == true) {
                    simViewer->disableExportToFile();
                } else {
                    simViewer->enableExportToFile(path, m_export->format());
                }
            } else {
                simViewer->disableExportToFile();
            }
        }

        return true;
    }


private slots:

    void load_defaults(int index=0) {
        double F;
        double k;

        if (index == 0) {
            m_defaults->setCurrentIndex(0);
            return;
        }

        if (index == 1) {
            F = 0.035;
            k = 0.065;
        } else if (index == 2) {
            F = 0.02;
            k = 0.055;
        } else {
            F = 0.035;
            k = 0.06;
        }

        QObject::disconnect(m_sliders["F"], SIGNAL(valueChanged()),
                this, SLOT(load_defaults()));
        QObject::disconnect(m_sliders["k"], SIGNAL(valueChanged()),
                this, SLOT(load_defaults()));

        m_sliders["F"]->setValue(F);
        m_sliders["k"]->setValue(k);

        QObject::connect(m_sliders["F"], SIGNAL(valueChanged()),
                this, SLOT(load_defaults()));
        QObject::connect(m_sliders["k"], SIGNAL(valueChanged()),
                this, SLOT(load_defaults()));
    }    


private:

    ExportWidget* m_export;
    QButtonGroup* m_speedup;
    QComboBox* m_defaults;
    QMap<QString, ParameterSlider*> m_sliders;

};


#endif // __PARAMETERBOX_HH__
