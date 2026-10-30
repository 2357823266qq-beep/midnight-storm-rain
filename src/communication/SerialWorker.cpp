#include "SerialWorker.h"
#include <QDebug>

SerialWorker::SerialWorker(QObject *parent) : QObject(parent)
{
    // 在构造函数中实例化，但不指定父对象
    // 这样方便后续将这个 Worker 整体 moveToThread
    m_serialPort = new QSerialPort(this);

    // 将底层串口的信号连接到当前类的槽函数
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialWorker::handleReadyRead);
    
    // 捕获串口底层错误
    connect(m_serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error){
        if (error != QSerialPort::NoError) {
            emit errorOccurred(m_serialPort->errorString());
        }
    });
}

SerialWorker::~SerialWorker()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

void SerialWorker::openPort(const QString &portName, int baudRate)
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        emit portOpened();
        qDebug() << "串口已打开:" << portName << "波特率:" << baudRate;
    } else {
        emit errorOccurred("无法打开串口: " + m_serialPort->errorString());
    }
}

void SerialWorker::closePort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        emit portClosed();
        qDebug() << "串口已关闭";
    }
}

void SerialWorker::sendData(const QByteArray &data)
{
    if (m_serialPort->isOpen()) {
        m_serialPort->write(data);
    } else {
        emit errorOccurred("串口未打开，无法发送数据");
    }
}

void SerialWorker::handleReadyRead()
{
    // 读取所有可用的数据，直接发给上层（协议解析层）
    QByteArray data = m_serialPort->readAll();
    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
}