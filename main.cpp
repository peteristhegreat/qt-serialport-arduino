#include <QCoreApplication>
#include "serialportmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SerialPortManager spm;

    spm.openSerialPort();

    return a.exec();
}
