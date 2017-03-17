/*
    PRACTICA 2 SISTEMAS OPERATIVOS AVANZADOS
    KAPIL ASHOK MELWANI CHUGANI
    UNIVERSIDAD DE LA LAGUNA
*/
#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QMainWindow>
#include <QThreadPool>
#include <QFileInfo>
#include <QFile>
#include <unistd.h>
#include <QTreeWidgetItem>
#include <QFutureWatcher>
#include <QFuture>
#include <QTimer>
#include <QStringList>
#include <QFutureSynchronizer>
#include <QTimer>
#include <QThread>
#include <QDir>
#include <QProcess>
#include <QtConcurrent>
#include <QStringList>
#include <QMetaType>
#include <QDebug>
#include <QMessageBox>
#include <QMutex>
#include <QWaitCondition>
#include <qjsonmodel.h>
#include <systemmonitorworker.h>
#include <systemmonitorcurrentusers.h>
#include <systemmonitorred.h>

namespace Ui {
class SystemMonitor;
}

class SystemMonitor : public QMainWindow
{
    Q_OBJECT

public:
    QStringList proc;
    explicit SystemMonitor(QWidget *parent = 0);
    ~SystemMonitor();
    //void listarSensores();
    //void addChild(QTreeWidgetItem *parent,QString name);

public slots:
    void processFinished();
    void listarHardware();
    QStringList listarProcesos();
    QString cpuinfo(void);

    void currentusers(void);
    void cpufinished();
    void redifconfig();
    void iniciar();
signals:
    void hardwareRequest();
    void sensorsRequest();
    void currentRequest();
    void redRequest();


private slots:
    void on_killButton_clicked();

private:
    Ui::SystemMonitor *ui;
    QThread hiloHardware;
    QThread hiloCurrent;
    QThread hiloRed;
    QFutureWatcher<QStringList> watcher;
    QFutureWatcher<QString> watcherCPU;
    QMutex mutex;
    QTimer timer;
    SystemMonitorCurrentUsers current;
    QFuture<QStringList> future;
    QFuture<QString> futureCPU;
    SystemMonitorRed red;
    SystemMonitorWorker Worker;
    QWaitCondition wait;

};

#endif // SYSTEMMONITOR_H
