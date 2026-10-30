cmake_minimum_required(VERSION 3.16)

project(DualChannelMonitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用 Qt 自动处理工具
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找系统安装的 Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets SerialPort Network Sql)

# 1. 添加可执行文件
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    core/DataType.h
)

# 2. 设置目标属性
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 链接 Qt5 的库
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Widgets 
    Qt5::SerialPort 
    Qt5::Network 
    Qt5::Sql
)