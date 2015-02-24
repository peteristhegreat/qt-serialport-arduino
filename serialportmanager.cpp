//#include "mainwindow.h"
//#include "ui_mainwindow.h"
//#include "console.h"
//#include "settingsdialog.h"

//#include <QMessageBox>
//#include <QtSerialPort/QSerialPort>

#include "serialportmanager.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

SerialPortManager::SerialPortManager(QObject *parent) :
    QObject(parent)
{

    // ui->setupUi(this);
    // console = new Console;
    // console->setEnabled(false);
    // setCentralWidget(console);

    serial = new QSerialPort(this);

    // settings = new SettingsDialog;

    // ui->actionConnect->setEnabled(true);
    // ui->actionDisconnect->setEnabled(false);
    // ui->actionQuit->setEnabled(true);
    // ui->actionConfigure->setEnabled(true);

    // initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));


    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    // connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));

}


SerialPortManager::~SerialPortManager()
{
    // delete settings;
    // delete ui;
}


void SerialPortManager::openSerialPort()
{
    QSerialPortInfo portToUse;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QString s = QObject::tr("Port:") + info.portName() + "\n"
                    + QObject::tr("Location:") + info.systemLocation() + "\n"
                    + QObject::tr("Description:") + info.description() + "\n"
                    + QObject::tr("Manufacturer:") + info.manufacturer() + "\n"
                    + QObject::tr("Serial number:") + info.serialNumber() + "\n"
                    + QObject::tr("Vendor Identifier:") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier:") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy:") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";

        if(!info.isBusy() && (info.description().contains("Arduino") || info.manufacturer().contains("Arduino")))
            portToUse = info;
        qDebug() << s;
    }

    if(portToUse.isNull() || !portToUse.isValid())
    {
        qDebug() << "port is not valid:" << portToUse.portName();
        return;
    }


    //    SettingsDialog::Settings p = settings->settings();
    // Enumerate the serial port
    // Find one that sounds like Arduino, or the highest one on the list
    // Open it if it isn't busy

    serial->setPortName(portToUse.portName());
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        // console->setEnabled(true);
        // console->setLocalEchoEnabled(p.localEchoEnabled);
        // ui->actionConnect->setEnabled(false);
        // ui->actionDisconnect->setEnabled(true);
        // ui->actionConfigure->setEnabled(false);
        qDebug() << "Connected to" << portToUse.description() << "on" << portToUse.portName();
    } else {
        qCritical() << "Serial Port error:" << serial->errorString();

        qDebug() << tr("Open error");
    }
}



void SerialPortManager::closeSerialPort()
{
    serial->close();
//    console->setEnabled(false);
    // ui->actionConnect->setEnabled(true);
    // ui->actionDisconnect->setEnabled(false);
    // ui->actionConfigure->setEnabled(true);
    qDebug() << tr("Disconnected");
}

void SerialPortManager::writeData(const QByteArray &data)
{
    serial->write(data);
}



void SerialPortManager::readData()
{
    QByteArray data = serial->readAll();
//    console->putData(data);
    qDebug() << "UART:" << data;
}



void SerialPortManager::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        // QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        qCritical() << "Serial Port error:" << serial->errorString();
        closeSerialPort();
    }
}


//void SerialPortManager::initActionsConnections()
//{
//    // connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
//    // connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
//    // connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
//    // connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
//    // connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
//}
