/*
    PRACTICA 2 SISTEMAS OPERATIVOS AVANZADOS
    KAPIL ASHOK MELWANI CHUGANI
    UNIVERSIDAD DE LA LAGUNA
*/

#ifndef SYSTEMMONITORCURRENTUSERS_H
#define SYSTEMMONITORCURRENTUSERS_H

#include <QString>
#include <QProcess>

class SystemMonitorCurrentUsers : public QObject
{
    Q_OBJECT
public:
    QString getLine()
    {
        return line;
    }

public slots:
    void doCurrentUsers()
    {
        QProcess users;
        users.start("w");
        users.waitForFinished();
        line = users.readAllStandardOutput();
        emit CurrentFinished();
    }

signals:
    void CurrentFinished();

 private:
    QString line;
};

#endif // SYSTEMMONITORCURRENTUSERS_H
