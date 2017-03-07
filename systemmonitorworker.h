#ifndef SYSTEMMONITORWORKER_H
#define SYSTEMMONITORWORKER_H

#include <QByteArray>
#include <QProcess>
#include <QDebug>


class SystemMonitorWorker : public QObject
{
    Q_OBJECT
public:
    QByteArray getByteArray()
    {
        return byteArray;
    }

public slots:
    void doHardware()
    {
        QProcess hardware;
        hardware.start("lshw -json");
        hardware.waitForFinished();
        byteArray = hardware.readAll();
        emit hardwareFinished();
    }

signals:
    void hardwareFinished();

 private:
    QByteArray byteArray;
};

#endif // SYSTEMMONITORWORKER_H
