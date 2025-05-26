# NPushButton 测试
cmake_minimum_required(VERSION 3.16)
project(tst_npushbutton LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Test)

add_executable(tst_npushbutton 
    tst_npushbutton.cpp
)

add_test(NAME tst_npushbutton COMMAND tst_npushbutton)

target_link_libraries(tst_npushbutton PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
    QtNativeUI::QtNativeUI
)
