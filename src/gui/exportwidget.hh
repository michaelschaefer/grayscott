#ifndef __EXPORTWIDGET_HH__
#define __EXPORTWIDGET_HH__


#include <QButtonGroup>
#include <QCheckBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include "src/gui/framedistributionwidget.hh"
#include "src/gui/parameterslider.hh"
#include "src/gui/text.hh"
#include "src/sim/parameters.hh"


class ExportWidget : public QGroupBox {

    Q_OBJECT


public:

    ExportWidget(QWidget* parent=0)
        : QGroupBox(parent)
    {
        setTitle(Text::Export::groupTitle());
        setCheckable(true);


        ParameterSlider* slider = new ParameterSlider(this, Text::Export::timesteps());
        slider->setFormat("%.0f");
        slider->setRange(0, 500000, 1000);
        slider->setValue(10000);
        m_sliders["timesteps"] = slider;

        slider = new ParameterSlider(this, Text::Export::frames());
        slider->setFormat("%.0f");
        slider->setRange(0, 10000, 1);
        slider->setValue(100);
        m_sliders["frames"] = slider;

        QObject::connect(m_sliders["timesteps"], SIGNAL(valueChanged()),
                this, SLOT(resetKeyframeSlider()));


        /*
         * Radio buttons for frame distribution
         */

        m_frameDistribution = new FrameDistributionWidget(this);


        /*
         * path box
         */

        m_buttonPath = new QPushButton(Text::Export::directoryButton());
        QObject::connect(m_buttonPath, SIGNAL(clicked()),
                         this, SLOT(getPath()));

        m_path = new QLineEdit();
        m_path->setReadOnly(true);
        m_path->setPlaceholderText(Text::Export::directoryPlaceholder());


        QGridLayout* box = new QGridLayout();
        box->addWidget(m_sliders["timesteps"], 0, 0, 1, 2);
        box->addWidget(m_sliders["frames"], 1, 0, 1, 2);
        box->addWidget(m_frameDistribution, 2, 0, 1, 2);
        box->addWidget(m_path, 3, 0);
        box->addWidget(m_buttonPath, 3, 1);
        setLayout(box);        
    }

    QString format() const {
        if (isChecked() == true) {
            return QString("png");
        } else {
            return QString();
        }
    }    

    void loadParameters() {
        setChecked(Parameters::Export::exportToFile);
        m_frameDistribution->setMode(Parameters::Export::frameDistributionMode);
        m_path->setText(Parameters::Export::path);
        m_sliders["timesteps"]->setValue(Parameters::Export::timesteps);
        m_sliders["frames"]->setValue(Parameters::Export::frames);
    }

    QString path() const {
        if (isChecked() == true) {
            return m_path->text();
        } else {
            return QString();
        }
    }

    bool saveParameters() {
        if (isChecked() == true) {
            if (m_path->text().isEmpty() == true) {
                QMessageBox info(parentWidget());
                info.setWindowTitle(Text::Application::name());
                info.setText(Text::Export::messageNoPathText());
                info.setStandardButtons(QMessageBox::Ok);
                info.show();
                info.exec();
                return false;
            }
        }

        Parameters::Export::exportToFile = isChecked();
        Parameters::Export::frameDistributionMode = m_frameDistribution->mode();
        Parameters::Export::frames = m_sliders["frames"]->value();
        Parameters::Export::path = m_path->text();
        Parameters::Export::timesteps = m_sliders["timesteps"]->value();

        return true;
    }


private slots:

    void getPath() {
        QString path = QFileDialog::getExistingDirectory(this->parentWidget());
        if (path.isNull() == false) {
            m_path->setText(path);
            m_path->setToolTip(path);
        }
    }

    void resetKeyframeSlider() {
        m_sliders["frames"]->setRange(1, m_sliders["timesteps"]->value());
    }


private:

    FrameDistributionWidget* m_frameDistribution;
    QMap<QString, ParameterSlider*> m_sliders;
    QPushButton* m_buttonPath;
    QLineEdit* m_path;

};

#endif // __EXPORTWIDGET_HH__
