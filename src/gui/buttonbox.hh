#ifndef __BUTTONBOX_HH__
#define __BUTTONBOX_HH__


#include <map>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>


using namespace std;


enum Button {
    Pause = 1,
    Resume = 2,
    Start = 4,
    Stop = 8
};


class ButtonBox : public QWidget {

    Q_OBJECT

signals:

    void clicked(Button);


public:

    ButtonBox(QWidget* parent=0)
        : QWidget(parent)
    {
        QPushButton* button = new QPushButton(this);
        button->setText("pause");
        QObject::connect(button, SIGNAL(clicked()), this, SLOT(emitPauseClicked()));
        m_buttons["pause"] = button;

        button = new QPushButton(this);
        button->setText("resume");
        QObject::connect(button, SIGNAL(clicked()), this, SLOT(emitResumeClicked()));
        m_buttons["resume"] = button;

        button = new QPushButton(this);
        button->setText("start");
        QObject::connect(button, SIGNAL(clicked()), this, SLOT(emitStartClicked()));
        m_buttons["start"] = button;

        button = new QPushButton(this);
        button->setText("stop");
        QObject::connect(button, SIGNAL(clicked()), this, SLOT(emitStopClicked()));
        m_buttons["stop"] = button;

        enableButton(Button::Pause | Button::Resume | Button::Start | Button::Stop);

        QHBoxLayout* box = new QHBoxLayout();
        box->addWidget(m_buttons["start"]);
        box->addWidget(m_buttons["pause"]);
        box->addWidget(m_buttons["resume"]);
        box->addWidget(m_buttons["stop"]);
        setLayout(box);
    }

    void enableButton(int buttons) {
        m_buttons["pause"]->setEnabled(buttons & Button::Pause);
        m_buttons["resume"]->setEnabled(buttons & Button::Resume);
        m_buttons["start"]->setEnabled(buttons & Button::Start);
        m_buttons["stop"]->setEnabled(buttons & Button::Stop);
    }


private slots:

    inline void emitPauseClicked() { emit clicked(Button::Pause); }
    inline void emitResumeClicked() { emit clicked(Button::Resume); }
    inline void emitStartClicked() { emit clicked(Button::Start); }
    inline void emitStopClicked() { emit clicked(Button::Stop); }


private:

    map<QString, QPushButton*> m_buttons;

};

#endif // __BUTTONBOX_HH__
