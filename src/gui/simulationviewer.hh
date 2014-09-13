#ifndef __SIMULATIONVIEWER_HH__
#define __SIMULATIONVIEWER_HH__


#include <QtCore/Qt>
#include <QFile>
#include <QFrame>
#include <QImage>
#include <QLabel>
#include <QRgb>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include "src/gui/colortable.hh"


typedef unsigned int uint;


class SimulationViewer : public QWidget {

    Q_OBJECT

public:

    SimulationViewer(QWidget* parent=0, uint size=512)
        : QWidget(parent)
    {
        m_size = size;
        m_colorTable = ColorTable::create(QColor(5, 48, 97),
                                          QColor(246, 246, 246),
                                          QColor(186, 61, 62));

        m_pixmapHolder = new QLabel(parent);
        m_pixmapHolder->setFixedSize(m_size, m_size);
        m_pixmapHolder->setFrameStyle(QFrame::Panel | QFrame::Sunken);

        m_statusLine = new QLabel(parent);
        m_statusLine->setText("no simulation running");

        QVBoxLayout* box = new QVBoxLayout();
        box->addWidget(m_pixmapHolder, 0, Qt::AlignHCenter);
        box->addWidget(m_statusLine, 0, Qt::AlignHCenter);
        setLayout(box);
    }

    void disableExportToFile() {
        m_exportToFile = false;
        m_exportPath.clear();
        m_exportFormat.clear();
    }

    void enableExportToFile(QString filename, QString format) {
        m_exportToFile = true;
        m_exportPath = filename;
        m_exportFormat = format;
    }

public slots:

    void updateImage(QImage image, uint frameNumber) {
        image.setColorTable(m_colorTable);
        QPixmap pixmap = QPixmap::fromImage(image);
        m_pixmapHolder->setPixmap(pixmap.scaled(m_size, m_size));

        if (m_exportToFile == true) {
            QString path = m_exportPath +
                    QString().sprintf("/%06d.", frameNumber) + m_exportFormat;
            QFile file(path);
            file.open(QIODevice::WriteOnly);
            m_pixmapHolder->pixmap()->save(&file, m_exportFormat.toStdString().c_str());
            file.close();
        }
    }

    void updateStatusLine(QString message) {
        m_statusLine->setText(message);
        m_statusLine->adjustSize();
    }


private:    

    bool m_exportToFile;

    uint m_size;

    QLabel* m_pixmapHolder;
    QLabel* m_statusLine;

    QString m_exportFormat;
    QString m_exportPath;

    QVector<QRgb> m_colorTable;
};


#endif // __SIMULATIONVIEWER_HH__
