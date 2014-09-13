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
#include "src/gui/distributionmodewidget.hh"
#include "src/gui/exportwidget.hh"
#include "src/gui/parameterslider.hh"
#include "src/gui/simulationviewer.hh"
#include "src/sim/modelparameters.hh"


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

        slider = new ParameterSlider(this, "timesteps");
        slider->setFormat("%.0f");
        slider->setRange(0, 500000, 1000);
        slider->setValue(10000);
        m_sliders["timesteps"] = slider;

        slider = new ParameterSlider(this, "keyframes");
        slider->setFormat("%.0f");
        slider->setRange(1, 1000, 1);
        slider->setValue(10);
        m_sliders["keyframes"] = slider;

        slider = new ParameterSlider(this, "size");
        slider->setFormat("%.0f");
        slider->setRange(32, 512, 32);
        slider->setValue(128);
        m_sliders["size"] = slider;


        /*
         * Combo box for default coefficient settings
         */

        m_defaults = new QComboBox(this);
        m_defaults->setEditable(false);
        m_defaults->addItem("<user-defined>");
        m_defaults->addItem("bacteria1");
        m_defaults->addItem("unstable");
        m_defaults->addItem("zebra");

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
         * Radio buttons for keyframe distribution
         */

        m_distributionMode = new DistributionModeWidget(this);


        /*
         * Export checkbox and path finder
         */

        m_export  = new ExportWidget(this);
        m_export->setChecked(false);


        /*
         * create and fill layout
         */

        QVBoxLayout* mainBox = new QVBoxLayout();

        QGroupBox* groupBox = new QGroupBox("System parameters");
        QVBoxLayout* box = new QVBoxLayout();
        box->addWidget(m_defaults);
        box->addWidget(m_sliders["F"]);
        box->addWidget(m_sliders["k"]);
        groupBox->setLayout(box);
        mainBox->addWidget(groupBox);

        groupBox = new QGroupBox("Visualization parameters");
        box = new QVBoxLayout();
        box->addWidget(m_sliders["timesteps"]);
        box->addWidget(m_sliders["keyframes"]);
        box->addWidget(m_sliders["size"]);
        box->addWidget(m_distributionMode);

        groupBox->setLayout(box);
        mainBox->addWidget(groupBox);

        mainBox->addWidget(m_export);

        setLayout(mainBox);
    }

    void loadParameters() {
        m_sliders["F"]->setValue(GrayScottModelParameters::F);
        m_sliders["k"]->setValue(GrayScottModelParameters::k);
        m_sliders["keyframes"]->setValue(GrayScottModelParameters::keyframes);
        m_sliders["size"]->setValue(GrayScottModelParameters::size);
        m_sliders["timesteps"]->setValue(GrayScottModelParameters::timesteps);
        m_distributionMode->setMode(GrayScottModelParameters::keyframeDistributionMode);
    }

    void forwardParameters(SimulationViewer* simViewer=0) {
        GrayScottModelParameters::F = m_sliders["F"]->value();
        GrayScottModelParameters::k = m_sliders["k"]->value();
        GrayScottModelParameters::keyframes = m_sliders["keyframes"]->value();
        GrayScottModelParameters::size = m_sliders["size"]->value();
        GrayScottModelParameters::timesteps = m_sliders["timesteps"]->value();        
        GrayScottModelParameters::keyframeDistributionMode = m_distributionMode->mode();

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

    DistributionModeWidget* m_distributionMode;
    ExportWidget* m_export;
    QComboBox* m_defaults;
    QMap<QString, ParameterSlider*> m_sliders;

};


#endif // __PARAMETERBOX_HH__
