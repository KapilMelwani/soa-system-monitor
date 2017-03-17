/*
	PRACTICA 2 SISTEMAS OPERATIVOS AVANZADOS
	KAPIL ASHOK MELWANI CHUGANI
	UNIVERSIDAD DE LA LAGUNA 
*/

#include "systemmonitor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SystemMonitor w;
    w.show();

    return a.exec();
}
