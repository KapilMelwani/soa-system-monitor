#ifndef SYSTEMMONITORCPU_H
#define SYSTEMMONITORCPU_H

#include <QObject>
#include <QByteArray>
#include <QProcess>
#include <QDir>
#include <QTextStream>


class SystemMonitorCPU : public QObject
{
    Q_OBJECT
public:
    QString getCpuLine();
public slots:
    QString doCPU()
    {
        QDir cpudir("/proc");
        //ui->labelCPU->setWordWrap(true);
        QFile cpuinfoFile(cpudir.absolutePath() + "/cpuinfo");
        if (cpuinfoFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            cpuline = cpuinfoFile.readLine();
            /*QTextStream stream(&cpuinfoFile);
            while (!stream.atEnd()){
                cpuline.append(stream.readLine()+"\n");
            }

            ui->labelCPU->setText(cpuline);*/
        }
        cpuinfoFile.close();
        return cpuline;
    }

signals:
    void CPUFinished();

 private:
    QString cpuline;
};

#endif // SYSTEMMONITORCPU_H
