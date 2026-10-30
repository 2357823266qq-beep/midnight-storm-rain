# 📡 双通道嵌入式设备监控与调试上位机 (Dual-Channel Embedded Monitor & Debugger)

[![C++](https://img.shields.io/badge/C++-11%2F17-blue.svg)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-5.15%2F6.x-green.svg)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()
[![Build](https://img.shields.io/badge/Build-CMake-orange.svg)]()

这是一个基于 **C++ / Qt** 开发的跨平台嵌入式设备监控与调试上位机。

针对工业现场常见的“双通道并发、高频数据交互、数据量庞大”等痛点，本项目采用**多线程架构**将通信与 UI 解耦，内置了**高可靠的流式协议解析引擎**，并实现了从数据采集、实时可视化、阈值告警到本地持久化的完整业务闭环。

## ✨ 核心功能 (Features)

- **🔌 双通道接入**：支持串口 (QSerialPort) 与 网络 TCP (QTcpSocket) 双通道同时接入，支持动态切换设备。
- **🧩 高可靠协议解析**：独立设计的协议解析模块，完美处理 TCP/串口的**粘包/拆包**问题，支持帧头识别、长度校验、CRC16 校验，将异构原始字节流统一转换为标准 JSON 格式。
- **⚡ 多线程架构**：采用 `QThread` + `moveToThread` 模式，将物理通信、协议解析、SQLite 存储等耗时操作全部移入后台工作线程，彻底杜绝高频数据导致的 UI 卡顿。
- **📈 高性能数据可视化**：集成 `QCustomPlot`，支持万级数据点的多通道实时曲线绘制、时间轴平滑滚动、动态阈值告警。
- **💾 本地数据持久化**：基于 SQLite 实现设备运行数据、操作日志与告警记录的异步批量写入，支持历史曲线回放与日志快速检索。
- **🛠️ 业务闭环控制**：实现设备指令下发、参数配置、断线自动重连、异常状态提示等完整的工业级交互逻辑。

## 🛠️ 技术栈 (Tech Stack)

- **语言与标准**：C++ 11/17
- **GUI 框架**：Qt Widgets
- **网络与串口**：QSerialPort, QTcpSocket, QNetwork
- **多线程**：QThread, QMutex, QSemaphore (线程安全与队列通信)
- **数据可视化**：QCustomPlot (引入第三方库)
- **数据存储**：SQLite (Qt SQL 模块)
- **数据格式**：JSON (QJsonDocument)
- **构建工具**：CMake

## 📂 项目架构 (Architecture)

项目采用分层架构设计，保证模块解耦和可扩展性：
```text
├── src/
│   ├── core/           # 核心业务逻辑
│   ├── communication/  # 通信层 (Serial, TCP 客户端/服务端实现)
│   ├── protocol/       # 协议层 (数据帧组装、拆包、CRC 校验)
│   ├── ui/             # 表现层 (主窗口、配置面板、波形图组件)
│   ├── storage/        # 存储层 (SQLite 数据库异步读写)
│   └── utils/          # 工具类 (日志管理、配置加载)
├── tests/              # 单元测试与模拟设备脚本 (Python/C++)
└── CMakeLists.txt
