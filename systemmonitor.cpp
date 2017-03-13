#include "systemmonitor.h"
#include "ui_systemmonitor.h"

SystemMonitor::SystemMonitor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SystemMonitor)
{

    qRegisterMetaType< QVector<int> >("QVector<int>");
    ui->setupUi(this);
    //Parte HARDWARE
    //QJsonModel *model = new QJsonModel;
    connect(&Worker,SIGNAL(hardwareFinished()),this,SLOT(listarHardware()));
    connect(this,SIGNAL(hardwareRequest()),&Worker,SLOT(doHardware()));
    emit hardwareRequest();
    Worker.moveToThread(&hiloHardware);
    hiloHardware.start();
    //Parte SENSORES
    emit sensorsRequest();
    //Parte CURRENT USERS
    connect(&current,SIGNAL(CurrentFinished()),this,SLOT(currentusers()));
    connect(this,SIGNAL(currentRequest()),&current,SLOT(doCurrentUsers()));
    emit currentRequest();
    current.moveToThread(&hiloCurrent);
    hiloCurrent.start();
    //Parte LISTAR PROCESOS
    future = QtConcurrent::run(this,&SystemMonitor::listarProcesos);
    timer.start(5000);
    watcher.setFuture(future);
    connect(&watcher,SIGNAL(finished()),this,SLOT(processFinished()));
    //cpuinfo();
}

SystemMonitor::~SystemMonitor()
{
    delete ui;
    hiloHardware.quit();
    hiloCurrent.quit();

}

void SystemMonitor::listarSensores()
{
    ui->treeSensores->setUniformRowHeights(true);
    QDir directorio("/sys/class/hwmon/hwmon0/");
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeSensores);
    QString content;
    QStringList filtro_temp;
    filtro_temp << "temp*";
    QStringList hwmon = directorio.entryList(filtro_temp);
    QString nombre;
    QFile nombreSensor(directorio.absolutePath() + "/name");
    if(nombreSensor.open(QIODevice::ReadOnly)){
        nombre = QString(nombreSensor.readAll());
        item->setText(0,nombre);
        ui->treeSensores->addTopLevelItem(item);
    }
    nombre.remove('\n');
    for(int i=0;i<hwmon.size();i++){
        QFile fileHwmon(directorio.absolutePath() + '/' + hwmon[i]);
        if(fileHwmon.open(QIODevice::ReadOnly))
            content = QString(fileHwmon.readAll());
            QString hwmonfinal = nombre + "::" + hwmon[i] + "->" + content;
        addChild(item,hwmonfinal);

    }
}

void SystemMonitor::addChild(QTreeWidgetItem *parent,QString name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0,name);
    parent->addChild(item);
}

QStringList SystemMonitor::listarProcesos()
{
    QStringList elements;
    QDir directorio("/proc");
    QStringList filtro;
    filtro << "[0-9]*";
    QString cmdline;
    QString name;
    QString state;
    QString threads;
    const char NAME_PROCESS[] = "Name:";
    const char THREADS_PROCESS[] = "Threads:";
    const char STATE_PROCESS[] = "State:";

    QStringList proc = directorio.entryList(filtro);

    for(int i=0;i<proc.size();i++){
        QFile cmdline_file(directorio.absolutePath() + '/' + proc[i] + "/cmdline");
        if(cmdline_file.open(QIODevice::ReadOnly)){
            cmdline = QString(cmdline_file.readAll());
        }
        QFile status_file(directorio.absolutePath() + '/' + proc[i] + "/status");

        status_file.open(QIODevice::ReadOnly);
            name = QString(status_file.readAll()).split("\n").filter(NAME_PROCESS).value(0).mid(sizeof(NAME_PROCESS));
        status_file.close();

        status_file.open(QIODevice::ReadOnly);
            state = QString(status_file.readAll()).split("\n").filter(STATE_PROCESS).value(0).mid(sizeof(STATE_PROCESS));
        status_file.close();

        status_file.open(QIODevice::ReadOnly);
            threads= QString(status_file.readAll()).split("\n").filter(THREADS_PROCESS).value(0).mid(sizeof(THREADS_PROCESS));
        status_file.close();
        elements.push_back(proc[i]);
        elements.push_back(cmdline);
        elements.push_back(name);
        elements.push_back(state);
        elements.push_back(threads);
    }
    return elements;

}

void SystemMonitor::currentusers()
{
    QString line = current.getLine();
    ui->labelusers->setText(line);
    qDebug()<<line;

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

void SystemMonitor::cpuinfo(void)
{
    QDir cpudir("/proc");
    ui->labelCPU->setWordWrap(true);
    QFile cpuinfoFile(cpudir.absolutePath() + "/cpuinfo");
    if (cpuinfoFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString cpuline = cpuinfoFile.readLine();
        QTextStream stream(&cpuinfoFile);
        while (!stream.atEnd()){
            cpuline.append(stream.readLine()+"\n");
        }
        ui->labelCPU->setText(cpuline);
    }
    cpuinfoFile.close();
}

void SystemMonitor::processFinished()
{
    int size;
    int rowPID = 0;
    int rowCMDLINE = 0;
    int rowSTATE = 0;
    int rowNAME = 0;
    int rowTHREADS = 0;
    QStringList elementsTableProcess = future.result();
    size = elementsTableProcess.size()/5;
    ui->tableProcesos->setRowCount(size);
    for(int i=0;i<elementsTableProcess.size();i+=5){
        ui->tableProcesos->setItem(rowPID,0,new QTableWidgetItem(elementsTableProcess[i]));
        rowPID++;
    }
    for(int j=1;j<elementsTableProcess.size();j+=5){
        if(elementsTableProcess[j] == ""){
            ui->tableProcesos->setItem(rowCMDLINE,1,new QTableWidgetItem("EMPTY"));
            rowCMDLINE++;
        }
        else{
            ui->tableProcesos->setItem(rowCMDLINE,1,new QTableWidgetItem(elementsTableProcess[j]));
            rowCMDLINE++;
        }
    }
    for(int k=2;k<elementsTableProcess.size();k+=5){
        ui->tableProcesos->setItem(rowSTATE,2,new QTableWidgetItem(elementsTableProcess[k]));
        rowSTATE++;
    }
    for(int l=3;l<elementsTableProcess.size();l+=5)
    {
        ui->tableProcesos->setItem(rowNAME,3,new QTableWidgetItem(elementsTableProcess[l]));
        rowNAME++;
    }
    for(int m=4;m<elementsTableProcess.size();m+=5)
    {
        ui->tableProcesos->setItem(rowTHREADS,4,new QTableWidgetItem(elementsTableProcess[m]));
        rowTHREADS++;
    }
}

void SystemMonitor::on_tabWidget_tabBarClicked(int index)
{
    switch(index){
    case 0:
        listarSensores();
        case 4:
            cpuinfo();
    }
}


