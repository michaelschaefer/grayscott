#ifndef __ABOUTDIALOG_HH__
#define __ABOUTDIALOG_HH__


#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QUrl>
#include <QWebView>
#include "src/gui/text.hh"


class AboutDialog : public QDialog {

public:

    enum Role {
        Model = 1,
        Program = 2
    };

    AboutDialog(QWidget* parent=0, Role role=Model)
        : QDialog(parent)
    {        
        m_webView = new QWebView(parent);
        m_webView->setHtml(getContent(role));
        m_webView->show();
        //m_webView->setStyleSheet("background: transparent");
        m_webView->setAttribute(Qt::WA_TranslucentBackground);

        m_close = new QPushButton(this);
        m_close->setText(Text::About::close());
        QObject::connect(m_close, SIGNAL(clicked()), this, SLOT(close()));

        QVBoxLayout* box = new QVBoxLayout();
        box->addWidget(m_webView);
        box->addWidget(m_close, 0, Qt::AlignRight);
        setLayout(box);
    }


private:

    QString getContent(Role role, bool defaultLocale=false) {
        QFile* file = 0;
        QString locale;

        if (defaultLocale == false) {
            locale = QLocale::system().name().mid(0, 2);
        } else {
            locale = "en";
        }

        if (role == Model) {
            file = new QFile(":/html/model_" + locale + ".html");
        } else {
            file = new QFile(":/html/program_" + locale + ".html");
        }

        if (file->open(QIODevice::ReadOnly) == true) {
            return QString(file->readAll());
        } else {
            if (defaultLocale == true) {
                return Text::About::dataNotFound();
            } else {
                QMessageBox::information(this, Text::Application::name(), Text::About::defaultLocaleText());
                return getContent(role, true);
            }
        }
    }


    QPushButton* m_close;
    QWebView* m_webView;

};

#endif // __ABOUTDIALOG_HH__
