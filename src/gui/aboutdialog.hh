#ifndef __ABOUTDIALOG_HH__
#define __ABOUTDIALOG_HH__


#include <QDialog>
#include <QVBoxLayout>
#include <QUrl>
#include <QWebView>


class AboutDialog : public QDialog {

public:

    enum Role {
        Model = 1,
        Program = 2
    };

    AboutDialog(QWidget* parent=0, Role role=Model)
        : QDialog(parent)
    {
        QString file = "file:///home/mscha_08/development/grayscott/doc/";
        if (role == Model) {
            file += "model.html";
        } else {
            file += "program.html";
        }

        m_webView = new QWebView(parent);
        m_webView->load(QUrl(file));
        m_webView->show();
        m_webView->setStyleSheet("background: transparent");
        m_webView->setAttribute(Qt::WA_TranslucentBackground);

        QVBoxLayout* box = new QVBoxLayout();
        box->addWidget(m_webView);
        setLayout(box);
    }


private:

    QWebView* m_webView;

};

#endif // __ABOUTDIALOG_HH__
