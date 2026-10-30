#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initUI();
    resize(1024, 768); // 给个初始大小
}

MainWindow::~MainWindow()
{
}

void MainWindow::initUI()
{
    // 先随便放个 Label 占位，后续我们会在这里拆分出控制面板和波形展示区
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    QLabel *placeholderLabel = new QLabel("双通道监控上位机框架已启动...\n等待接入通信线程与 UI 组件", this);
    placeholderLabel->setAlignment(Qt::AlignCenter);
    
    // 设置字体大一点
    QFont font = placeholderLabel->font();
    font.setPointSize(16);
    placeholderLabel->setFont(font);

    layout->addWidget(placeholderLabel);
    setCentralWidget(centralWidget);
}