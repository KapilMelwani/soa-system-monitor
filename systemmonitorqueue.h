#ifndef SYSTEMMONITORQUEUE_H
#define SYSTEMMONITORQUEUE_H
#include <QMainWindow>
#include <QPair>
#include <QVector>
#include <QString>
#include <QList>

class SystemMonitorQueue : public QMainWindow
{
    Q_OBJECT
    QString hwmon;
    QString fileName;
    QList<QPair<QString,int>> lista;
public:
    SystemMonitorQueue(QString hwmon,QString fileName,QList<QPair<QString,int>> lista);
    ~SystemMonitorQueue();

    QString getHwmon();
    QString getfileName();
    QList<QPair<QString,int>> getLista();

    void setHwmon(QString hwmon);
    void setfileName(QString fileName);
    void setLista(QList<QPair<QString,int>> lista);
};

#endif // SYSTEMMONITORQUEUE_H
