#ifndef SYSTEMMONITORRED_H
#define SYSTEMMONITORRED_H

#include <QProcess>

class SystemMonitorRed : public QObject
{
    Q_OBJECT
public slots:
    QString doRed()
    {
        QProcess red;
        red.start("/sbin/ifconfig");
        red.waitForFinished();
    }
signals:
    void redFinished();
};

#endif // SYSTEMMONITORRED_H
