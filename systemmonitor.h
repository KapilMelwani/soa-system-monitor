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
    void addRoot(QString name);

public slots:
    void processFinished();
    void listarHardware();
    QStringList listarProcesos();


signals:
    void hardwareRequest();

private slots:
    void on_killButton_clicked();
    void on_restartButton_clicked();
  //  void on_tabWidget_tabBarClicked(int index);

private:
    Ui::SystemMonitor *ui;
    mythread thread;
    QThread hilo_hardware;
    QMutex mutex;
    QFuture<QStringList> future;
    SystemMonitorWorker Worker;
    QWaitCondition wait;

};

#endif // SYSTEMMONITOR_H
