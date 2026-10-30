#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "communication/SerialWorker.h"

// 前向声明 UI 控件
class QComboBox;
class QPushButton;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void initUI();
    void refreshPorts();
    void onOpenPortClicked();
    void onClosePortClicked();
    void onSendDataClicked();
    void onDataReceived(const QByteArray &data); // 接收串口数据
    void onErrorOccurred(const QString &errorString); // 接收错误信息

private:
    // UI 控件
    QComboBox *m_portNameComboBox;
    QComboBox *m_baudRateComboBox;
    QPushButton *m_refreshBtn;
    QPushButton *m_openPortBtn;
    QPushButton *m_closePortBtn;
    QPushButton *m_sendBtn;
    QTextEdit *m_logTextEdit;

    // 线程与 Worker
    QThread *m_serialThread;
    SerialWorker *m_serialWorker;
};

#endif // MAINWINDOW_H