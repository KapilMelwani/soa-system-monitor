/*
    PRACTICA 2 SISTEMAS OPERATIVOS AVANZADOS
    KAPIL ASHOK MELWANI CHUGANI
    UNIVERSIDAD DE LA LAGUNA
*/

#ifndef SYSTEMMONITORRED_H
#define SYSTEMMONITORRED_H


#include <QByteArray>
#include <QProcess>
#include <QDebug>

class SystemMonitorRed : public QObject
{
    Q_OBJECT
public:
    QString getLine()
    {
        return redline;
    }

public slots:
    void doRed()
    {
        QProcess red;
        red.start("/sbin/ifconfig");
        red.waitForFinished();
        redline = red.readAllStandardOutput();
        emit redFinished();
    }

signals:
    void redFinished();

 private:
    QString redline;
};

#endif // SYSTEMMONITORRED_H


