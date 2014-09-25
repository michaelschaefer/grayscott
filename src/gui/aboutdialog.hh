#ifndef __ABOUTDIALOG_HH__
#define __ABOUTDIALOG_HH__


#include <QDesktopServices>
#include <QDialog>
#include <QFile>
#include <QLocale>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QVBoxLayout>
#include <QUrl>
#include <QWebView>
#include "src/gui/text.hh"


class AboutDialog : public QDialog {

    Q_OBJECT


public:

    enum Role {
        Model = 1,
        Program = 2
    };

    AboutDialog(QWidget* parent=0, Role role=Model)
        : QDialog(parent)
    {
        QUrl url = getContentUrl(role);

        m_webView = new QWebView(this);
        if (url.isEmpty() == true) {
            m_webView->setHtml(Text::About::dataNotFound());
        } else {
            m_webView->load(url);
        }

        m_webView->show();        
        QPalette palette = m_webView->palette();
        palette.setBrush(QPalette::Base, Qt::transparent);
        m_webView->page()->setPalette(palette);
        m_webView->setAttribute(Qt::WA_OpaquePaintEvent, false);
        m_webView->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
        QObject::connect(m_webView, SIGNAL(linkClicked(QUrl)),
                         this, SLOT(openLink(QUrl)));

        m_close = new QPushButton(this);
        m_close->setText(Text::About::close());
        QObject::connect(m_close, SIGNAL(clicked()), this, SLOT(close()));

        QVBoxLayout* box = new QVBoxLayout();        
        box->addWidget(m_webView);
        box->addWidget(m_close, 0, Qt::AlignRight);
        setLayout(box);
    }


private slots:

    void openLink(QUrl url) {
        QDesktopServices::openUrl(url);
    }


private:

    QUrl getContentUrl(Role role, bool defaultLocale=false) {
        QString filename;
        QString locale;
        QUrl url;

        if (defaultLocale == false) {
            locale = QLocale::system().name().mid(0, 2);
        } else {
            locale = "en";
        }

        if (role == Model) {
            filename = ":/html/model_" + locale + ".html";
        } else {
            filename = ":/html/program_" + locale + ".html";
        }        

        if (QFile(filename).open(QIODevice::ReadOnly) == true) {
            return QUrl("qrc" + filename);
        } else {
            if (defaultLocale == true) {
                return QUrl();
            } else {
                QMessageBox::information(this, Text::Application::name(), Text::About::defaultLocaleText());
                return getContentUrl(role, true);
            }
        }
    }    


    QPushButton* m_close;
    QWebView* m_webView;

};

#endif // __ABOUTDIALOG_HH__
