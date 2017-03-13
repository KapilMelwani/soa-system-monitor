#include "systemmonitorqueue.h"

SystemMonitorQueue::SystemMonitorQueue(QString hwmon, QString fileName, QList<QPair<QString,int>> lista)
{
    hwmon = hwmon;
    fileName = fileName;
    lista = lista;
}

SystemMonitorQueue::~SystemMonitorQueue()
{

}
