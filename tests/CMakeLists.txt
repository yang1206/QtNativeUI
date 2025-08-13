# 启用测试
enable_testing()

# 查找Qt组件
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Test)

# 设置Qt自动化工具
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 简化的测试函数 - 不再需要src参数，直接使用name.cpp
function(add_qtnativeui_test name)
    add_executable(${name} ${name}.cpp)

    # 设置包含目录
    target_include_directories(${name} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/private
    )

    # 链接库
    target_link_libraries(${name} PRIVATE
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Gui
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Test
            QtNativeUI::QtNativeUI
    )

    # 添加测试
    add_test(NAME ${name} COMMAND ${name})

    # 设置测试属性
    set_target_properties(${name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )

    # 设置测试超时
    set_tests_properties(${name} PROPERTIES
            TIMEOUT 60
    )
endfunction()

add_qtnativeui_test(tst_npushbutton)
add_qtnativeui_test(tst_ncalendardatepicker)
add_qtnativeui_test(tst_nlineedit)
add_qtnativeui_test(tst_ntoolbutton)
