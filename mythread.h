#include <QThread>
#include <QProcess>

#ifndef MYTHREAD_H
#define MYTHREAD_H


class mythread : public QThread
{
    Q_OBJECT

public:
    mythread();

protected:
    void run();


};

#endif // MYTHREAD_H
