cmake_minimum_required(VERSION 3.16)

project(QtNativeUIExamples LANGUAGES CXX)

# 设置CMake变量
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 查找Qt依赖
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(EXAMPLE_PAGES_SOURCES
        pages/BasePage.cpp
        pages/BasePage.h
        pages/basic/LabelPage.cpp
        pages/basic/LabelPage.h
        pages/basic/PushButtonPage.cpp
        pages/basic/PushButtonPage.h
        pages/basic/ToolButtonPage.cpp
        pages/basic/ToolButtonPage.h
        pages/basic/HyperlinkButtonPage.cpp
        pages/basic/HyperlinkButtonPage.h
        pages/basic/ToggleButtonPage.cpp
        pages/basic/ToggleButtonPage.h
        pages/basic/DropDownButtonPage.cpp
        pages/basic/DropDownButtonPage.h
        pages/input/LineEditPage.cpp
        pages/input/LineEditPage.h
        pages/input/TextEditPage.cpp
        pages/input/TextEditPage.h
        pages/input/PlainTextEditPage.cpp
        pages/input/PlainTextEditPage.h
        pages/input/AutoSuggestBoxPage.cpp
        pages/input/AutoSuggestBoxPage.h
        pages/input/ComboBoxPage.cpp
        pages/input/ComboBoxPage.h
        pages/input/SpinBoxPage.cpp
        pages/input/SpinBoxPage.h
        pages/input/DoubleSpinBoxPage.cpp
        pages/input/DoubleSpinBoxPage.h
        pages/input/CheckBoxPage.cpp
        pages/input/CheckBoxPage.h
        pages/input/RadioButtonPage.cpp
        pages/input/RadioButtonPage.h
        pages/input/ToggleSwitchPage.cpp
        pages/input/ToggleSwitchPage.h
        pages/input/SliderPage.cpp
        pages/input/SliderPage.h
        pages/input/PickerPage.cpp
        pages/input/PickerPage.h
        pages/datetime/CalendarWidgetPage.cpp
        pages/datetime/CalendarWidgetPage.h
        pages/datetime/CalendarDatePickerPage.cpp
        pages/datetime/CalendarDatePickerPage.h
        pages/datetime/DatePickerPage.cpp
        pages/datetime/DatePickerPage.h
        pages/datetime/TimePickerPage.cpp
        pages/datetime/TimePickerPage.h
        pages/container/GroupBoxPage.cpp
        pages/container/GroupBoxPage.h
        pages/container/ScrollAreaPage.cpp
        pages/container/ScrollAreaPage.h
        pages/container/StackedWidgetPage.cpp
        pages/container/StackedWidgetPage.h
        pages/container/OverlayPage.cpp
        pages/container/OverlayPage.h
        pages/navigation/NavigationViewPage.cpp
        pages/navigation/NavigationViewPage.h
        pages/navigation/TabWidgetPage.cpp
        pages/navigation/TabWidgetPage.h
        pages/navigation/TabBarPage.cpp
        pages/navigation/TabBarPage.h
        pages/navigation/PivotPage.cpp
        pages/navigation/PivotPage.h
        pages/navigation/MenuPage.cpp
        pages/navigation/MenuPage.h
        pages/feedback/ContentDialogPage.cpp
        pages/feedback/ContentDialogPage.h
        pages/feedback/FlyoutPage.cpp
        pages/feedback/FlyoutPage.h
        pages/feedback/InfoBarPage.cpp
        pages/feedback/InfoBarPage.h
        pages/feedback/ToolTipPage.cpp
        pages/feedback/ToolTipPage.h
        pages/feedback/ProgressBarPage.cpp
        pages/feedback/ProgressBarPage.h
        pages/feedback/ProgressRingPage.cpp
        pages/feedback/ProgressRingPage.h
        pages/list/ListViewPage.cpp
        pages/list/ListViewPage.h
        pages/list/TreeViewPage.cpp
        pages/list/TreeViewPage.h
        pages/window/DialogPage.cpp
        pages/window/DialogPage.h
        pages/window/MainWindowPage.cpp
        pages/window/MainWindowPage.h
        pages/window/WindowBarPage.cpp
        pages/window/WindowBarPage.h
        pages/window/ScrollBarPage.cpp
        pages/window/ScrollBarPage.h
        pages/tool/ColorsPage.cpp
        pages/tool/ColorsPage.h
        pages/tool/IconsPage.cpp
        pages/tool/IconsPage.h
        pages/settings/SettingsPage.cpp
        pages/settings/SettingsPage.h
)


set(EXAMPLE_MAIN_SOURCES
        mainwindow.h
        mainwindow.cpp
        main.cpp
)

# 合并所有源文件
set(EXAMPLE_SOURCES
        ${EXAMPLE_PAGES_SOURCES}
        ${EXAMPLE_MAIN_SOURCES}
)

# 创建可执行文件
qt_add_executable(QtNativeUIExamples
        ${EXAMPLE_SOURCES}
        MANUAL_FINALIZATION
)

# 简化MOC设置
set_target_properties(QtNativeUIExamples PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON

)
target_compile_definitions(QtNativeUIExamples
        PRIVATE
        QT_SHARED
)

# 包含二进制目录以找到生成的导出头文件
target_include_directories(QtNativeUIExamples
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)

# 链接QtNativeUI库
target_link_libraries(QtNativeUIExamples
        PRIVATE
        QtNativeUI::QtNativeUI
        Qt${QT_VERSION_MAJOR}::Widgets
)

# 设置编译选项
if (MSVC)
    target_compile_options(QtNativeUIExamples
            PRIVATE
            /W4
            /utf-8
    )
endif ()

# 完成Qt设置
qt_finalize_executable(QtNativeUIExamples)