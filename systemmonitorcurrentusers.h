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
    QString doCurrentUsers()
    {
        QProcess users;
        users.start("w");
        users.waitForFinished();
        line = users.readAllStandardOutput();
        emit CurrentFinished();
        return line;
    }

signals:
    void CurrentFinished();

 private:
    QString line;
};

#endif // SYSTEMMONITORCURRENTUSERS_H
