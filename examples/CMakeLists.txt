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
        pages/buttons/PushButtonPage.cpp
        pages/buttons/PushButtonPage.h
        pages/buttons/ToolButtonPage.cpp
        pages/buttons/ToolButtonPage.h
        pages/buttons/ToggleButtonPage.cpp
        pages/buttons/ToggleButtonPage.h
        pages/buttons/HyperlinkButtonPage.cpp
        pages/buttons/HyperlinkButtonPage.h
        pages/buttons/DropDownButtonPage.cpp
        pages/buttons/DropDownButtonPage.h
        pages/inputs/LabelPage.cpp
        pages/inputs/LabelPage.h
        pages/inputs/LineEditPage.cpp
        pages/inputs/LineEditPage.h
        pages/inputs/TextEditPage.cpp
        pages/inputs/TextEditPage.h
        pages/inputs/PlainTextEditPage.cpp
        pages/inputs/PlainTextEditPage.h
        pages/inputs/AutoSuggestBoxPage.cpp
        pages/inputs/AutoSuggestBoxPage.h
        pages/inputs/SpinBoxPage.cpp
        pages/inputs/SpinBoxPage.h
        pages/inputs/DoubleSpinBoxPage.cpp
        pages/inputs/DoubleSpinBoxPage.h
        pages/inputs/ComboBoxPage.cpp
        pages/inputs/ComboBoxPage.h
        pages/inputs/PickerPage.cpp
        pages/inputs/PickerPage.h
        pages/selections/CheckBoxPage.cpp
        pages/selections/CheckBoxPage.h
        pages/selections/RadioButtonPage.cpp
        pages/selections/RadioButtonPage.h
        pages/selections/ToggleSwitchPage.cpp
        pages/selections/ToggleSwitchPage.h
        pages/selections/SliderPage.cpp
        pages/selections/SliderPage.h
        pages/datetime/CalendarWidgetPage.cpp
        pages/datetime/CalendarWidgetPage.h
        pages/datetime/CalendarDatePickerPage.cpp
        pages/datetime/CalendarDatePickerPage.h
        pages/datetime/TimePickerPage.cpp
        pages/datetime/TimePickerPage.h
        pages/progress/ProgressBarPage.cpp
        pages/progress/ProgressBarPage.h
        pages/progress/ProgressRingPage.cpp
        pages/progress/ProgressRingPage.h
        pages/containers/GroupBoxPage.cpp
        pages/containers/GroupBoxPage.h
        pages/containers/ScrollAreaPage.cpp
        pages/containers/ScrollAreaPage.h
        pages/containers/StackedWidgetPage.cpp
        pages/containers/StackedWidgetPage.h
        pages/containers/OverlayPage.cpp
        pages/containers/OverlayPage.h
        pages/navigation/NavigationViewPage.cpp
        pages/navigation/NavigationViewPage.h
        pages/navigation/TabWidgetPage.cpp
        pages/navigation/TabWidgetPage.h
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
        pages/windows/DialogPage.cpp
        pages/windows/DialogPage.h
        pages/windows/MainWindowPage.cpp
        pages/windows/MainWindowPage.h
        pages/windows/WindowBarPage.cpp
        pages/windows/WindowBarPage.h
        pages/windows/ScrollBarPage.cpp
        pages/windows/ScrollBarPage.h
        pages/tools/ColorsPage.cpp
        pages/tools/ColorsPage.h
        pages/tools/IconsPage.cpp
        pages/tools/IconsPage.h
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