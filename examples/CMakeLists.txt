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


set(EXAMPLE_COMPONENTS_SOURCES
    components/button.cpp
    components/button.h
    components/colors.cpp
    components/colors.h
    components/controls.cpp
    components/controls.h
    components/dialogs.cpp
    components/dialogs.h
    components/icon.cpp
    components/icon.h
    components/menu.cpp
    components/menu.h
    components/navigation.cpp
    components/navigation.h
    components/view.cpp
    components/view.h
)

set(EXAMPLE_WIDGETS_SOURCES
    widgets/ExampleSection.cpp
    widgets/ExampleSection.h
)

set(EXAMPLE_MAIN_SOURCES
    mainwindow.h
    mainwindow.cpp
    main.cpp
)

# 合并所有源文件
set(EXAMPLE_SOURCES
    ${EXAMPLE_COMPONENTS_SOURCES}
    ${EXAMPLE_WIDGETS_SOURCES}
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