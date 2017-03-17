# System Monitor

Con la ayuda de QtCreator me he servido para crear un SystemMonitor el cual nos ofrezca informacion sobre nuestro ordenador/portatil mediante el uso de hilos

# SystemMonitorTabs

## Lista de procesos

Teniendo en cuenta que en nuestro sistema Linux, se almacenan los procesos del sistema en la carpera /proc, hemos utilizado el metodo de hilos que nos ofrece QtConcurrent que consiste en crear un Pool de hilos, los cuales se encargaran de mostrarnos cada uno de los procesos que se estan ejecutando en nuestro sistema, para ello, mediante la señal que nos ofrece el QFutureWatcher -> finished(), ejecutaremos un SLOT el cual devolvera la salida del QFuture mediante -> future.result(). 

Hemos decidido mostrar:

+ PID del proceso en ejecucion
+ CMDLINE
+ NAME
+ STATE
+ THREADS

Aqui podremos ver una version final de nuestro listar procesos en un QTableWidget

![Ventana Listar Procesos](/../master/ListarProcesos.png)


Ademas se decidio añadir un boton "Kill ALL Process" el cual mata todos los procesos del sistema. Para que funcione debemos entrar en el codigo de systemmonitor.cpp y cambiar el nombre de usuario, es decir, pkill -u kali mata todos los procesos del usuario "kali", por tanto, para que funcione, tenemos que cambiar "kali" por el nombre de usuario de tu sistema Linux.

```
void SystemMonitor::on_killButton_clicked()
{
    QProcess p;
    p.start("pkill -u kali");
    p.waitForFinished(-1);
}

```

NOTA: No asustarse si tarda en mostrarse los procesos, el programa esta hecho para que cada 5 segundos se muestren los procesos en ejecucion. Eso se ha realizado mediante el uso de un QTimer.

## Hardware

Hemos creado un hilo de ejecucion en la clase SystemMonitorWorker el cual ejecuta mediante QProcess el comando lshw -json el cual nos devuelve como salida un -json de los elementos hardware que tenemos en el sistema. Para ello, creamos la clase nombrada anteriormente y le conectamos unas señales desde systemmonitor.cpp a esta clase para que cuando systemmonitor emita la señal de que quiere listar los elementos hardware, systemmonitorworker se ponga en marcha, asi, cuando termine, devolvera una señal a systemmonitor para que este sepa que ha terminado, mostrandose asi los elementos en un QTreeWidget mediante un SLOT que podremos contemprar en systemmonitor.cpp

Ademas, cabe destacar el uso de la clase qjsonmodel el cual nos permitio añadir el -json automaticamente al QTreeWidget. Aqui os dejo el link en github https://github.com/dridk/QJsonModel

Vemos una version final del listar Hardware:

![Ventana Listar Hardware](/../master/ListarHardware.png)

## CPU

De la misma forma que se creo el Pool de hilos en Listar Procesos, hemos utilizado la misma herramienta de hilos para mostrar la informacion mas relevante de nuestra CPU. (Esta informacion está almacenada en /proc/cpuinfo)

Hemos decidido mostrar de ese fichero:

+ Processor 
+ Model Name 
+ Cache Size

Veamos una Version final del mostrar cpuinfo.

![Ventana Listar CPUInfo](/../master/ListarCPUInfo.png)


## Current Users

Al igual que se realizo en Listar Hardware, hemos utilizado QProcess para ejecutar el comando "w" el cual nos devolvera los usuarios que actualmente han iniciado sesion en la computadora.

systemmonitor -> manda señal de listar current users a -> systemmonitorcurrentusers
SystemMonitorCurrentUsers ejecuta el comando...
systemmonitorcurrentusers -> manda señal de listar current a finalizado a -> systemmonitor
systemmonitor ejecuta slot para mostrarlo en un QLabel

Veamos la version final de listar current users

![Ventana Listar Current Users](/../master/ListarCurrentUsers.png)

## Red

Al igual que el anterior, se muestra en texto plano en un QLabel la salida del comando ifconfig.

systemmonitor -> manda señal de listar red a -> systemmonitorred
SystemMonitorRed ejecuta el comando...
systemmonitorred -> manda señal de listar red a finalizado a -> systemmonitor
systemmonitor ejecuta slot para mostrarlo en un QLabel

Veamos la version final de mostrar red.

![Ventana Listar Red](/../master/ListarRed.png)

## Requisitos

1. Tener un ordenador/laptop con un sistema operativo LINUX
2. Tener descargado en tu ordenador QtCreator
3. Descargar este codigo

## Ejecución

1. Descargar este codigo
2. Abrirlo en QtCreator
3. Darle al botón "play" de QtCreator para ver el Application Output

## Hilos

En el uso de las herramientas que nos ofrece QtCreator en la gestion de hilos, no puedo definir cual es el mejor de los dos que he usado puesto que tienen finalidades distintas. En mi opinion, usaria el QtConcurrent mas que el crear un hilo, moverlo en otra clase... conectar las señales con esa clase, puesto que es mas rapido y facil de utilizar puesto que podemos mostrar la salida del future cuando nosotros querramos, ademas de poder actualizarlo facilmente con un QFutureWatcher. En conclusion, utilizaria mas QtConcurrent que cualquier otra herramienta de hilos, es rapida (te crea un pool de hilos...), es bastante escalable y con alta calidad.



