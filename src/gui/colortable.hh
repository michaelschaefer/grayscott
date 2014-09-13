#ifndef __COLORTABLE_HH__
#define __COLORTABLE_HH__


#include <QColor>
#include <QRgb>
#include <QVector>


class ColorTable {

public:

    static QVector<QRgb> create(QColor start, QColor mid, QColor end) {
        double fraction;
        int endRed, endGreen, endBlue;
        int midRed, midGreen, midBlue;
        int startRed, startGreen, startBlue;
        int curRed, curGreen, curBlue;
        QVector<QRgb> table;

        end.getRgb(&endRed, &endGreen, &endBlue);
        mid.getRgb(&midRed, &midGreen, &midBlue);
        start.getRgb(&startRed, &startGreen, &startBlue);

        for (int i = 0; i < 256; ++i) {
            if (i < 128) {
                fraction = i / 128.0;
                curRed = startRed + fraction * (midRed - startRed);
                curGreen = startGreen + fraction * (midGreen - startGreen);
                curBlue = startBlue + fraction * (midBlue - startBlue);
            } else {
                fraction = (i - 128) / 127.0;
                curRed = midRed + fraction * (endRed - midRed);
                curGreen = midGreen + fraction * (endGreen - midGreen);
                curBlue = midBlue + fraction * (endBlue - midBlue);
            }
            table.append(QColor(curRed, curGreen, curBlue).rgb());
        }

        return table;
    }


    static QVector<QRgb> BlueWhiteRed;
    static QVector<QRgb> RedWhiteBlue;
};


#endif // __COLORTABLE_HH__
