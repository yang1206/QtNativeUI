# 最小CMake版本
cmake_minimum_required(VERSION 3.16)

# 设置项目名称
set(TARGET_NAME tst_ntoolbutton)

# 设置Qt模块
find_package(Qt6 REQUIRED COMPONENTS Test Widgets)

# 自动生成moc文件
set(CMAKE_AUTOMOC ON)

# 添加测试可执行文件
add_executable(${TARGET_NAME} tst_ntoolbutton.cpp)

# 链接Qt库
target_link_libraries(${TARGET_NAME} PRIVATE
    Qt6::Test
    Qt6::Widgets
    QtNativeUI
)

# 添加到CTest
add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
