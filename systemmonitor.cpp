#include "systemmonitor.h"
#include "ui_systemmonitor.h"

SystemMonitor::SystemMonitor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SystemMonitor)
{

    qRegisterMetaType< QVector<int> >("QVector<int>");
    ui->setupUi(this);
    ui->tabWidget->removeTab(0);
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
    timer.start(5000);
    connect(&timer,SIGNAL(timeout()),this,SLOT(iniciar()));
    //Parte RED
    connect(&red,SIGNAL(redFinished()),this,SLOT(redifconfig()));
    connect(this,SIGNAL(redRequest()),&red,SLOT(doRed()));
    emit redRequest();
    red.moveToThread(&hiloRed);
    hiloRed.start();
    //Parte CPUINFO
    futureCPU = QtConcurrent::run(this,&SystemMonitor::cpuinfo);
    watcherCPU.setFuture(futureCPU);
    connect(&watcherCPU,SIGNAL(finished()),this,SLOT(cpufinished()));
}

SystemMonitor::~SystemMonitor()
{
    delete ui;
    hiloHardware.quit();
    hiloHardware.wait();
    hiloCurrent.quit();
    hiloCurrent.wait();
    hiloRed.quit();
    hiloRed.wait();
}

/*----------------------------PARTE LISTAR PROCESOS----------------------------*/

void SystemMonitor::iniciar()
{
    future = QtConcurrent::run(this,&SystemMonitor::listarProcesos);
    watcher.setFuture(future);
    connect(&watcher,SIGNAL(finished()),this,SLOT(processFinished()));
    timer.start(5000);
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


void SystemMonitor::on_killButton_clicked()
{
    QProcess p;
    p.start("pkill -u kali");
    p.waitForFinished(-1);
}


/*--------------------------PARTE LISTAR RED----------------------------*/

void SystemMonitor::redifconfig()
{
    QString redline = red.getLine();
    ui->labelRed->setText(redline);
}

/*--------------------------PARTE CURRENT USERS----------------------------*/

void SystemMonitor::currentusers()
{
    QString line = current.getLine();
    ui->labelusers->setText(line);
}

/*--------------------------PARTE HARDWARE----------------------------*/

void SystemMonitor::listarHardware()
{
    QByteArray bA;
    QJsonModel *model = new QJsonModel;
    bA = Worker.getByteArray();
    ui->treeHardware->setModel(model);
    model->loadJson(bA);
}

/*--------------------------PARTE CPU INFO----------------------------*/


void SystemMonitor::cpufinished()
{
    qDebug()<<futureCPU.result();
    QString resultCpu = futureCPU.result();
    ui->labelCPU->setText(resultCpu);
}

QString SystemMonitor::cpuinfo(void)
{
    QString cpuline;
    QDir cpudir("/proc");
    //ui->labelCPU->setWordWrap(true);
    QFile cpuinfoFile(cpudir.absolutePath() + "/cpuinfo");
    if (cpuinfoFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        while (true) {
            QString line = cpuinfoFile.readLine();
            if (line.isEmpty()) break;
            auto items = line.split("\t:");
            if (items[0] == "model name")
                cpuline.append(line+"\n");
            if(items[0] == "processor")
                cpuline.append(line+"\n");
            if(items[0] == "cache size")
                cpuline.append(line+"\n");
        }
    }
    cpuinfoFile.close();
    return cpuline;
}


/*--------------------------PARTE MANUAL (SIN HILOS) SENSORES ----------------------------*/

/*
void SystemMonitor::listarSensores()
{
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
*/

