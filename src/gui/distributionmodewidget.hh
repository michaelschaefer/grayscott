#ifndef __DISTRIBUTIONMODEWIDGET_HH__
#define __DISTRIBUTIONMODEWIDGET_HH__


#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMap>
#include <QRadioButton>
#include <QWidget>
#include "src/sim/keyframestepper.hh"


class DistributionModeWidget : public QWidget {

public:

    DistributionModeWidget(QWidget* parent=0)
        : QWidget(parent)
    {
        QRadioButton* radioButton = new QRadioButton("linear");
        radioButton->setChecked(true);
        m_radioButtons["linear"] = radioButton;

        radioButton = new QRadioButton("logarithmic");
        radioButton->setChecked(false);
        m_radioButtons["logarithmic"] = radioButton;

        radioButton = new QRadioButton("squared");
        radioButton->setChecked(false);
        m_radioButtons["squared"] = radioButton;

        m_buttonGroup = new QButtonGroup();
        m_buttonGroup->addButton(m_radioButtons["linear"],
                KeyframeStepper::Linear);
        m_buttonGroup->addButton(m_radioButtons["logarithmic"],
                KeyframeStepper::Logarithmic);
        m_buttonGroup->addButton(m_radioButtons["squared"],
                KeyframeStepper::Squared);

        QGroupBox* groupBox = new QGroupBox("Keyframe distribution mode",
                                            parent);
        QHBoxLayout* box = new QHBoxLayout();
        box->addWidget(m_radioButtons["linear"]);
        box->addWidget(m_radioButtons["logarithmic"]);
        box->addWidget(m_radioButtons["squared"]);
        groupBox->setLayout(box);

        box = new QHBoxLayout();
        box->addWidget(groupBox);
        setLayout(box);
    }

    inline KeyframeStepper::DistributionMode mode() const {
        int id = m_buttonGroup->checkedId();
        switch (id) {
        case KeyframeStepper::Squared:
            return KeyframeStepper::Squared;
        case KeyframeStepper::Logarithmic:
            return KeyframeStepper::Logarithmic;
        default:
            return KeyframeStepper::Linear;
        }
    }

    void setMode(KeyframeStepper::DistributionMode mode) {
        if (mode == KeyframeStepper::Linear) {
            m_radioButtons["linear"]->setChecked(true);
        } else if (mode == KeyframeStepper::Logarithmic) {
            m_radioButtons["logarithmic"]->setChecked(true);
        } else {
            m_radioButtons["squared"]->setChecked(true);
        }
    }


private:

    QButtonGroup* m_buttonGroup;
    QMap<QString, QRadioButton*> m_radioButtons;

};


#endif // __DISTRIBUTIONMODEWIDGET_HH__
