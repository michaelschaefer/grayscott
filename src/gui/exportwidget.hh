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
#include <QPushButton>
#include <QRadioButton>


class ExportWidget : public QGroupBox {

    Q_OBJECT

public:

    ExportWidget(QWidget* parent=0)
        : QGroupBox(parent)
    {
        setTitle("export keyframes");
        setCheckable(true);

        m_buttonPath = new QPushButton("choose directory");
        QObject::connect(m_buttonPath, SIGNAL(clicked()),
                         this, SLOT(getPath()));

        /*
         * format box
         */

        QHBoxLayout* groupLayout = new QHBoxLayout();
        m_formatButtonGroup = new QButtonGroup();

        QRadioButton* radioButton = new QRadioButton("PNG");
        radioButton->setChecked(true);
        groupLayout->addWidget(radioButton);
        m_formatButtonGroup->addButton(radioButton);

        radioButton = new QRadioButton("PPM");
        groupLayout->addWidget(radioButton);
        m_formatButtonGroup->addButton(radioButton);

        m_formatBox = new QGroupBox("Export format");
        m_formatBox->setLayout(groupLayout);

        /*
         * path box
         */

        m_path = new QLineEdit();
        m_path->setReadOnly(true);
        m_path->setPlaceholderText("no directory selected");        

        QGridLayout* box = new QGridLayout();
        box->addWidget(m_path, 0, 0);
        box->addWidget(m_buttonPath, 1, 0);
        box->addWidget(m_formatBox, 2, 0);
        setLayout(box);        
    }

    QString format() const {
        if (isChecked() == true) {
            return m_formatButtonGroup->checkedButton()->text().toLower();
        } else {
            return QString();
        }
    }

    QString path() const {
        if (isChecked() == true) {
            return m_path->text();
        } else {
            return QString();
        }
    }


private slots:

    void getPath() {
        QString path = QFileDialog::getExistingDirectory(this->parentWidget());
        if (path.isNull() == false) {
            m_path->setText(path);
        }
    }


private:

    QButtonGroup* m_formatButtonGroup;
    QGroupBox* m_formatBox;
    QPushButton* m_buttonPath;
    QLineEdit* m_path;

};

#endif // __EXPORTWIDGET_HH__
