#ifndef __FRAMEDISTRIBUTIONWIDGET_HH__
#define __FRAMEDISTRIBUTIONWIDGET_HH__


#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMap>
#include <QRadioButton>
#include <QWidget>
#include "src/gui/text.hh"
#include "src/sim/framedistribution.hh"


class FrameDistributionWidget : public QWidget {

public:

    FrameDistributionWidget(QWidget* parent=0)
        : QWidget(parent)
    {
        QRadioButton* radioButton = new QRadioButton(Text::FrameDistribution::linear());
        radioButton->setChecked(true);
        m_radioButtons["linear"] = radioButton;

        radioButton = new QRadioButton(Text::FrameDistribution::logarithmic());
        radioButton->setChecked(false);
        m_radioButtons["logarithmic"] = radioButton;

        radioButton = new QRadioButton(Text::FrameDistribution::quadratic());
        radioButton->setChecked(false);
        m_radioButtons["quadratic"] = radioButton;

        m_buttonGroup = new QButtonGroup();
        m_buttonGroup->addButton(m_radioButtons["linear"],
                FrameDistribution::Linear);
        m_buttonGroup->addButton(m_radioButtons["logarithmic"],
                FrameDistribution::Logarithmic);
        m_buttonGroup->addButton(m_radioButtons["quadratic"],
                FrameDistribution::Quadratic);

        QGroupBox* groupBox = new QGroupBox(Text::FrameDistribution::groupTitle(),
                                            parent);
        QHBoxLayout* box = new QHBoxLayout();
        box->addWidget(m_radioButtons["linear"]);
        box->addWidget(m_radioButtons["logarithmic"]);
        box->addWidget(m_radioButtons["quadratic"]);
        groupBox->setLayout(box);

        box = new QHBoxLayout();
        box->addWidget(groupBox);
        setLayout(box);
    }

    inline FrameDistribution::Mode mode() const {
        int id = m_buttonGroup->checkedId();
        switch (id) {
        case FrameDistribution::Quadratic:
            return FrameDistribution::Quadratic;
        case FrameDistribution::Logarithmic:
            return FrameDistribution::Logarithmic;
        default:
            return FrameDistribution::Linear;
        }
    }

    void setMode(FrameDistribution::Mode mode) {
        if (mode == FrameDistribution::Linear) {
            m_radioButtons["linear"]->setChecked(true);
        } else if (mode == FrameDistribution::Logarithmic) {
            m_radioButtons["logarithmic"]->setChecked(true);
        } else {
            m_radioButtons["quadratic"]->setChecked(true);
        }
    }


private:

    QButtonGroup* m_buttonGroup;
    QMap<QString, QRadioButton*> m_radioButtons;

};


#endif // __FRAMEDISTRIBUTIONWIDGET_HH__
