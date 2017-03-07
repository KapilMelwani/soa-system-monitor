#include "systemmonitor.h"
#include "ui_systemmonitor.h"

SystemMonitor::SystemMonitor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SystemMonitor)
{

    qRegisterMetaType< QVector<int> >("QVector<int>");
    ui->setupUi(this);
    connect(&Worker,SIGNAL(hardwareFinished()),this,SLOT(listarHardware()));
    connect(this,SIGNAL(hardwareRequest()),&Worker,SLOT(doHardware()));
    emit hardwareRequest();
    QFutureWatcher<QStringList> watcher;
    future = QtConcurrent::run(this,&SystemMonitor::listarProcesos);
    watcher.setFuture(future);
    connect(&watcher,SIGNAL(finished()),this,SLOT(processFinished()));

}

SystemMonitor::~SystemMonitor()
{
    delete ui;
}

void SystemMonitor::addRoot(QString name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeSensores);
    item->setText(0,name);
    ui->treeSensores->addTopLevelItem(item);
}

void SystemMonitor::listarSensores()
{
    QDir directorio("/sys/class/hwmon/hwmon0/");
    QStringList hwmon = directorio.entryList();
    qDebug()<<hwmon;
    for(int i=0;i<hwmon.size();i++)
        addRoot(hwmon[i]);


}

QStringList SystemMonitor::listarProcesos()
{
    QStringList elements;
    QDir directorio("/proc");
    QStringList filtro;
    filtro << "[0-9]*";
    QString cmdline;
    QString cmdline_empty = "EMPTY";
    QString name;
    QString state;
    QString threads;
    const char NAME_PROCESS[] = "Name:";
    const char THREADS_PROCESS[] = "Threads:";
    const char STATE_PROCESS[] = "State:";

    QStringList proc = directorio.entryList(filtro);
    //ui->tableProcesos->setRowCount(proc.size());

    for(int i=0;i<proc.size();i++){

        //ui->tableProcesos->setItem(i,0,new QTableWidgetItem(proc[i]));

        QFile cmdline_file(directorio.absolutePath() + '/' + proc[i] + "/cmdline");
        if(cmdline_file.open(QIODevice::ReadOnly)){
            cmdline = QString(cmdline_file.readAll());
            //if(cmdline.isEmpty())
                //ui->tableProcesos->setItem(i,1,new QTableWidgetItem(cmdline_empty));
            //else
                //ui->tableProcesos->setItem(i,1,new QTableWidgetItem(cmdline));
        }
        QFile status_file(directorio.absolutePath() + '/' + proc[i] + "/status");

        status_file.open(QIODevice::ReadOnly);
            name = QString(status_file.readAll()).split("\n").filter(NAME_PROCESS).value(0).mid(sizeof(NAME_PROCESS));
            //ui->tableProcesos->setItem(i,2,new QTableWidgetItem(name));
        status_file.close();

        status_file.open(QIODevice::ReadOnly);
            state = QString(status_file.readAll()).split("\n").filter(STATE_PROCESS).value(0).mid(sizeof(STATE_PROCESS));
            //ui->tableProcesos->setItem(i,3,new QTableWidgetItem(state));
        status_file.close();

        status_file.open(QIODevice::ReadOnly);
            threads= QString(status_file.readAll()).split("\n").filter(THREADS_PROCESS).value(0).mid(sizeof(THREADS_PROCESS));
            //ui->tableProcesos->setItem(i,4,new QTableWidgetItem(threads));
        status_file.close();
        elements.push_back(proc[i]);
        elements.push_back(cmdline);
        elements.push_back(name);
        elements.push_back(state);
        elements.push_back(threads);
    }
    sleep(1);
    return elements;

}

void SystemMonitor::listarHardware()
{
    QByteArray bA;
    QJsonModel *model = new QJsonModel;
    bA = Worker.getByteArray();
    ui->treeHardware->setModel(model);
    model->loadJson(bA);
}


void SystemMonitor::on_killButton_clicked()
{
    QProcess p;
    p.start("pkill -u kali");
    p.waitForFinished(-1);

}

void SystemMonitor::on_restartButton_clicked()
{
    QProcess sudo;
    QProcess restart_process;
    sudo.start("xterm");
    sudo.waitForFinished(-1);
}

void SystemMonitor::processFinished()
{
    QStringList elementsTableProcess = future.result();
    ui->tableProcesos->insertRow(elementsTableProcess.size());
    for(int i=0;i<elementsTableProcess.size();i+=5)
        ui->tableProcesos->setItem(i,0,new QTableWidgetItem(elementsTableProcess[i]));
    for(int i=1;i<elementsTableProcess.size();i+=5)
        ui->tableProcesos->setItem(i,1,new QTableWidgetItem(elementsTableProcess[i]));
    for(int i=2;i<elementsTableProcess.size();i+=5)
        ui->tableProcesos->setItem(i,2,new QTableWidgetItem(elementsTableProcess[i]));
    for(int i=3;i<elementsTableProcess.size();i+=5)
        ui->tableProcesos->setItem(i,3,new QTableWidgetItem(elementsTableProcess[i]));
    for(int i=4;i<elementsTableProcess.size();i+=5)
        ui->tableProcesos->setItem(i,4,new QTableWidgetItem(elementsTableProcess[i]));
}

/*void SystemMonitor::on_tabWidget_tabBarClicked(int index)
{
    switch(index){
        case 0:
            listarSensores();
            break;
        case 1:
            listarProcesos();
            break;
        case 2:
            break;
        case 3:
            listarHardware();
            break;
    }
}
*/
