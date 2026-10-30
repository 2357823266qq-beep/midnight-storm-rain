#ifndef DATATYPE_H
#define DATATYPE_H

#include <QMetaType>
#include <QString>
#include <QDateTime>

// 传感器数据结构体
struct SensorData {
    int channelId = 0;      // 通道号 (例如 1 代表串口，2 代表 TCP)
    double temperature = 0.0;
    double humidity = 0.0;
    QDateTime timestamp;
};

// 设备告警结构体
struct DeviceAlarm {
    int level = 0;          // 告警级别 (0: 提示, 1: 警告, 2: 严重)
    QString message;        // 告警信息
    QDateTime timestamp;
};

// 注册为 Qt 元类型 (放在头文件末尾)
Q_DECLARE_METATYPE(SensorData)
Q_DECLARE_METATYPE(DeviceAlarm)

#endif // DATATYPE_H