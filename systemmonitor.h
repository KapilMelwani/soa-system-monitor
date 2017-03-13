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
#include <mythread.h>
#include <qjsonmodel.h>
#include <systemmonitorworker.h>
#include <systemmonitorcurrentusers.h>

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
    void listarSensores();
    void addChild(QTreeWidgetItem *parent,QString name);
    void cpuinfo(void);


public slots:
    void processFinished();
    void listarHardware();
    QStringList listarProcesos();
    void currentusers(void);



signals:
    void hardwareRequest();
    void sensorsRequest();
    void currentRequest();

private slots:
    void on_killButton_clicked();
    void on_restartButton_clicked();
    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::SystemMonitor *ui;
    QThread hiloHardware;
    QThread hiloCurrent;
    QFutureWatcher<QStringList> watcher;
    QMutex mutex;
    QTimer timer;
    SystemMonitorCurrentUsers current;
    QFuture<QStringList> future;
    SystemMonitorWorker Worker;
    QWaitCondition wait;

};

#endif // SYSTEMMONITOR_H
