#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 1. 初始化界面
    initUI();

    // 2. 初始化子线程和 Worker
    m_serialThread = new QThread(this);
    m_serialWorker = new SerialWorker();
    
    // 【核心】把 Worker 移动到子线程
    m_serialWorker->moveToThread(m_serialThread);

    // 3. 连接跨线程的信号槽
    // 接收 Worker 发出的数据，传给主界面的 TextEdit
    connect(m_serialWorker, &SerialWorker::dataReceived, this, &MainWindow::onDataReceived);
    connect(m_serialWorker, &SerialWorker::errorOccurred, this, &MainWindow::onErrorOccurred);
    
    // 更新 UI 状态
    connect(m_serialWorker, &SerialWorker::portOpened, this, [this](){
        m_openPortBtn->setEnabled(false);
        m_closePortBtn->setEnabled(true);
        m_logTextEdit->append("✅ 串口已打开");
    });
    connect(m_serialWorker, &SerialWorker::portClosed, this, [this](){
        m_openPortBtn->setEnabled(true);
        m_closePortBtn->setEnabled(false);
        m_logTextEdit->append("❌ 串口已关闭");
    });

    // 启动线程 (进入事件循环)
    m_serialThread->start();
}

MainWindow::~MainWindow()
{
    // 安全退出线程的标准流程
    m_serialThread->quit();
    m_serialThread->wait();
}

void MainWindow::initUI()
{
    resize(800, 600);
    QWidget *centralWidget = new QWidget(this);
    
    // 顶部控制面板
    QHBoxLayout *ctrlLayout = new QHBoxLayout();
    m_portNameComboBox = new QComboBox();
    m_baudRateComboBox = new QComboBox();
    m_baudRateComboBox->addItems({"9600", "115200", "921600"});
    m_baudRateComboBox->setCurrentText("115200");
    
    m_refreshBtn = new QPushButton("刷新串口");
    m_openPortBtn = new QPushButton("打开串口");
    m_closePortBtn = new QPushButton("关闭串口");
    m_closePortBtn->setEnabled(false);
    
    ctrlLayout->addWidget(new QLabel("端口:"));
    ctrlLayout->addWidget(m_portNameComboBox);
    ctrlLayout->addWidget(m_refreshBtn);
    ctrlLayout->addWidget(new QLabel("波特率:"));
    ctrlLayout->addWidget(m_baudRateComboBox);
    ctrlLayout->addWidget(m_openPortBtn);
    ctrlLayout->addWidget(m_closePortBtn);

    // 中间日志面板
    m_logTextEdit = new QTextEdit();
    m_logTextEdit->setReadOnly(true);

    // 底部发送面板
    QHBoxLayout *sendLayout = new QHBoxLayout();
    m_sendBtn = new QPushButton("发送测试数据");
    sendLayout->addWidget(m_sendBtn);

    // 组合总布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(ctrlLayout);
    mainLayout->addWidget(m_logTextEdit);
    mainLayout->addLayout(sendLayout);
    
    setCentralWidget(centralWidget);

    // 绑定槽函数
    connect(m_refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshPorts);
    connect(m_openPortBtn, &QPushButton::clicked, this, &MainWindow::onOpenPortClicked);
    connect(m_closePortBtn, &QPushButton::clicked, this, &MainWindow::onClosePortClicked);
    connect(m_sendBtn, &QPushButton::clicked, this, &MainWindow::onSendDataClicked);

    // 初始化下拉框
    refreshPorts();
}

// ==========================================
// 下面这些就是报错里说“找不到”的具体实现
// 如果你没复制下面这些，就会报 undefined reference
// ==========================================

void MainWindow::refreshPorts()
{
    m_portNameComboBox->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        m_portNameComboBox->addItem(info.portName());
    }
}

void MainWindow::onOpenPortClicked()
{
    if (m_portNameComboBox->currentText().isEmpty()) return;
    
    QString portName = m_portNameComboBox->currentText();
    int baudRate = m_baudRateComboBox->currentText().toInt();
    
    // 【核心】跨线程调用 Worker 的槽函数，Qt 会自动使用 QueuedConnection
    QMetaObject::invokeMethod(m_serialWorker, "openPort", Qt::QueuedConnection,
                              Q_ARG(QString, portName), Q_ARG(int, baudRate));
}

void MainWindow::onClosePortClicked()
{
    QMetaObject::invokeMethod(m_serialWorker, "closePort", Qt::QueuedConnection);
}

void MainWindow::onSendDataClicked()
{
    QByteArray testData = "Hello Embedded Device!";
    QMetaObject::invokeMethod(m_serialWorker, "sendData", Qt::QueuedConnection,
                              Q_ARG(QByteArray, testData));
    m_logTextEdit->append("发送: " + QString(testData));
}

void MainWindow::onDataReceived(const QByteArray &data)
{
    // 由于跨线程，这里已经在主线程了，可以安全更新 UI
    m_logTextEdit->append("接收原始数据: " + QString(data));
}

void MainWindow::onErrorOccurred(const QString &errorString)
{
    m_logTextEdit->append("⚠️ 错误: " + errorString);
}