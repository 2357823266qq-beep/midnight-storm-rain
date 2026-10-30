#include <QApplication>
#include <QDebug>
#include <QMetaType>

#include "MainWindow.h"
#include "core/DataType.h"

int main(int argc, char *argv[])
{
    // 1. 初始化 Qt 应用程序
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication a(argc, argv);

    // 2. 设置应用全局信息 (用于 QSettings 或后续日志模块)
    a.setApplicationName("DualChannelMonitor");
    a.setApplicationVersion("1.0.0");
    a.setOrganizationName("EmbeddedLab");

    // 3. 【关键】注册自定义数据类型到 Qt 元对象系统
    qRegisterMetaType<DeviceAlarm>("DeviceAlarm");
    qRegisterMetaType<QList<SensorData>>("QList<SensorData>");

    // 4. 实例化并显示主窗口
    MainWindow w;
    w.setWindowTitle("双通道嵌入式设备监控与调试上位机");
    w.show();

    // 5. 进入 Qt 主事件循环
    return a.exec();
}