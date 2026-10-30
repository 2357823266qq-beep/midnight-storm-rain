#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>

class SerialWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialWorker(QObject *parent = nullptr);
    ~SerialWorker();

public slots:
    // 打开/关闭串口 (由主线程通过信号槽调用)
    void openPort(const QString &portName, int baudRate = 115200);
    void closePort();
    
    // 发送数据 (由主线程调用)
    void sendData(const QByteArray &data);

signals:
    // 收到数据信号 (发给协议解析器或主线程)
    void dataReceived(const QByteArray &data);
    
    // 状态变化信号 (用于 UI 更新状态灯)
    void portOpened();
    void portClosed();
    
    // 错误信号
    void errorOccurred(const QString &errorString);

private slots:
    // 串口底层的 readyRead 信号触发此槽函数
    void handleReadyRead();

private:
    QSerialPort *m_serialPort;
};

#endif // SERIALWORKER_H