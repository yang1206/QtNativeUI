cmake_minimum_required(VERSION 3.16)

project(QtNativeUI
        VERSION 1.0.0
        DESCRIPTION "A simple and easy-to-use UI library for Qt"
        HOMEPAGE_URL "https://github.com/yang1206/QtNativeUI"
        LANGUAGES CXX
)

# 设置CMake选项
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(QtNativeUI_BUILD_EXAMPLES "Build QtNativeUI examples" ON)
option(QtNativeUI_BUILD_TESTS "Build QtNativeUI tests" ON)

# 设置CMake变量
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找依赖
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# QWindowKit 配置 - 作为 subproject 引入
set(QWINDOWKIT_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(QWINDOWKIT_BUILD_WIDGETS ON CACHE BOOL "" FORCE)
set(QWINDOWKIT_BUILD_QUICK OFF CACHE BOOL "" FORCE)
set(QWINDOWKIT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(QWINDOWKIT_BUILD_DOCUMENTATIONS OFF CACHE BOOL "" FORCE)
set(QWINDOWKIT_INSTALL OFF CACHE BOOL "" FORCE)
set(QWINDOWKIT_ENABLE_STYLE_AGENT ON CACHE BOOL "" FORCE)
add_subdirectory(third_party/qwindowkit)


set(QtNativeUI_CONFIG_SOURCES
    src/config/NConfig.cpp
    src/config/NNavigationRouter.cpp
)

set(QtNativeUI_CONTROLS_SOURCES
    src/controls/NAnimation.cpp
    src/controls/NAutoSuggestBox.cpp
    src/controls/NCalendarDatePicker.cpp
    src/controls/NCalendarWidget.cpp
    src/controls/NCheckBox.cpp
    src/controls/NComboBox.cpp
    src/controls/NContentDialog.cpp
    src/controls/NDialog.cpp
    src/controls/NDoubleSpinBox.cpp
    src/controls/NDropDownButton.cpp
    src/controls/NFlyout.cpp
    src/controls/NGroupBox.cpp
    src/controls/NHyperlinkButton.cpp
    src/controls/NIcon.cpp
    src/controls/NInfoBar.cpp
    src/controls/NLabel.cpp
    src/controls/NLineEdit.cpp
    src/controls/NListView.cpp
    src/controls/NMainWindow.cpp
    src/controls/NMenu.cpp
    src/controls/NMenuBar.cpp
    src/controls/NNavigationBar.cpp
    src/controls/NNavigationView.cpp
    src/controls/NOverlay.cpp
    src/controls/NPageComponent.cpp
    src/controls/NPivot.cpp
    src/controls/NPlainTextEdit.cpp
    src/controls/NProgressBar.cpp
    src/controls/NProgressRing.cpp
    src/controls/NPushButton.cpp
    src/controls/NRadioButton.cpp
    src/controls/NRangeSlider.cpp
    src/controls/NScrollArea.cpp
    src/controls/NScrollBar.cpp
    src/controls/NSlider.cpp
    src/controls/NSpinBox.cpp
    src/controls/NStackedWidget.cpp
    src/controls/NTabBar.cpp
    src/controls/NTabWidget.cpp
    src/controls/NTextEdit.cpp
    src/controls/NToggleButton.cpp
    src/controls/NToggleSwitch.cpp
    src/controls/NToolButton.cpp
    src/controls/NToolTip.cpp
    src/controls/NTreeView.cpp
    src/controls/NWindowBar.cpp
    src/controls/NWindowButton.cpp
)

set(QtNativeUI_PRIVATE_SOURCES
    src/private/nanimation_p.cpp
    src/private/nanimation_p.h
    src/private/nautosuggestbox_p.cpp
    src/private/nautosuggestbox_p.h
    src/private/nautosuggestmodel.cpp
    src/private/nautosuggestmodel.h
    src/private/nbaselistview.cpp
    src/private/nbaselistview.h
    src/private/ncalendardatepicker_p.cpp
    src/private/ncalendardatepicker_p.h
    src/private/ncalendardelegate.cpp
    src/private/ncalendardelegate.h
    src/private/ncalendarheader.cpp
    src/private/ncalendarheader.h
    src/private/ncalendarmodel.cpp
    src/private/ncalendarmodel.h
    src/private/ncalendarwidget_p.cpp
    src/private/ncalendarwidget_p.h
    src/private/ncheckbox_p.cpp
    src/private/ncheckbox_p.h
    src/private/ncombobox_p.cpp
    src/private/ncombobox_p.h
    src/private/ncomboboxstyle.cpp
    src/private/ncomboboxstyle.h
    src/private/ncontentdialog_p.cpp
    src/private/ncontentdialog_p.h
    src/private/ndialog_p.cpp
    src/private/ndialog_p.h
    src/private/ndoublespinbox_p.cpp
    src/private/ndoublespinbox_p.h
    src/private/ndropdownbutton_p.cpp
    src/private/ndropdownbutton_p.h
    src/private/neditstyle.cpp
    src/private/neditstyle.h
    src/private/nflyout_p.cpp
    src/private/nflyout_p.h
    src/private/nframelesshelper_p.cpp
    src/private/nframelesshelper_p.h
    src/private/ngroupbox_p.cpp
    src/private/ngroupbox_p.h
    src/private/nhyperlinkbutton_p.cpp
    src/private/nhyperlinkbutton_p.h
    src/private/ninfobar_p.cpp
    src/private/ninfobar_p.h
    src/private/nlabel_p.cpp
    src/private/nlabel_p.h
    src/private/nlineedit_p.cpp
    src/private/nlineedit_p.h
    src/private/nlistview_p.cpp
    src/private/nlistview_p.h
    src/private/nlistviewstyle_p.cpp
    src/private/nlistviewstyle_p.h
    src/private/nmainwindow_p.cpp
    src/private/nmainwindow_p.h
    src/private/nmenu_p.cpp
    src/private/nmenu_p.h
    src/private/nnavigationanimation_p.cpp
    src/private/nnavigationanimation_p.h
    src/private/nnavigationbar_p.cpp
    src/private/nnavigationbar_p.h
    src/private/nnavigationfooterdelegate_p.cpp
    src/private/nnavigationfooterdelegate_p.h
    src/private/nnavigationfootermodel_p.cpp
    src/private/nnavigationfootermodel_p.h
    src/private/nnavigationmanager.cpp
    src/private/nnavigationmanager.h
    src/private/nnavigationmodel_p.cpp
    src/private/nnavigationmodel_p.h
    src/private/nnavigationnode_p.cpp
    src/private/nnavigationnode_p.h
    src/private/nnavigationstyle_p.cpp
    src/private/nnavigationstyle_p.h
    src/private/nnavigationtreeview_p.cpp
    src/private/nnavigationtreeview_p.h
    src/private/nnavigationview_p.cpp
    src/private/nnavigationview_p.h
    src/private/noverlay_p.cpp
    src/private/noverlay_p.h
    src/private/npivot_p.cpp
    src/private/npivot_p.h
    src/private/nplaintextedit_p.cpp
    src/private/nplaintextedit_p.h
    src/private/nprogressbar_p.cpp
    src/private/nprogressbar_p.h
    src/private/nprogressring_p.cpp
    src/private/nprogressring_p.h
    src/private/npushbutton_p.cpp
    src/private/npushbutton_p.h
    src/private/nradiobutton_p.cpp
    src/private/nradiobutton_p.h
    src/private/nrangeslider_p.cpp
    src/private/nrangeslider_p.h
    src/private/nscrollarea_p.cpp
    src/private/nscrollarea_p.h
    src/private/nscrollbar_p.cpp
    src/private/nscrollbar_p.h
    src/private/nscrollbarstyle.cpp
    src/private/nscrollbarstyle.h
    src/private/nslider_p.cpp
    src/private/nslider_p.h
    src/private/nspinbox_p.cpp
    src/private/nspinbox_p.h
    src/private/nspinboxstyle.cpp
    src/private/nspinboxstyle.h
    src/private/ntabbar_p.cpp
    src/private/ntabbar_p.h
    src/private/ntabbarstyle.cpp
    src/private/ntabbarstyle.h
    src/private/ntabwidget_p.cpp
    src/private/ntabwidget_p.h
    src/private/ntextedit_p.cpp
    src/private/ntextedit_p.h
    src/private/ntheme_p.cpp
    src/private/ntheme_p.h
    src/private/ntogglebutton_p.cpp
    src/private/ntogglebutton_p.h
    src/private/ntoggleswitch_p.cpp
    src/private/ntoggleswitch_p.h
    src/private/ntoolbutton_p.cpp
    src/private/ntoolbutton_p.h
    src/private/ntooltip_p.cpp
    src/private/ntooltip_p.h
    src/private/ntranslation.cpp
    src/private/ntranslation.h
    src/private/ntreeview_p.cpp
    src/private/ntreeview_p.h
    src/private/ntreeviewstyle_p.cpp
    src/private/ntreeviewstyle_p.h
    src/private/nwindowbar_p.h
    src/private/nwindowbutton_p.h
)

set(QtNativeUI_THEME_SOURCES
    src/theme/NColor.cpp
    src/theme/NTheme.cpp
)

# 合并所有源文件
set(QtNativeUI_SOURCES
    ${QtNativeUI_CONFIG_SOURCES}
    ${QtNativeUI_CONTROLS_SOURCES}
    ${QtNativeUI_PRIVATE_SOURCES}
    ${QtNativeUI_THEME_SOURCES}
)

# 公共头文件
set(QtNativeUI_HEADERS
    include/QtNativeUI/NAnimation.h
    include/QtNativeUI/NAutoSuggestBox.h
    include/QtNativeUI/NCalendarDatePicker.h
    include/QtNativeUI/NCalendarWidget.h
    include/QtNativeUI/NCheckBox.h
    include/QtNativeUI/NColor.h
    include/QtNativeUI/NComboBox.h
    include/QtNativeUI/NConfig.h
    include/QtNativeUI/NContentDialog.h
    include/QtNativeUI/NDialog.h
    include/QtNativeUI/NDoubleSpinBox.h
    include/QtNativeUI/NDropDownButton.h
    include/QtNativeUI/NEnums.h
    include/QtNativeUI/NFluentColors.h
    include/QtNativeUI/NFlyout.h
    include/QtNativeUI/NGroupBox.h
    include/QtNativeUI/NHyperlinkButton.h
    include/QtNativeUI/NIcon.h
    include/QtNativeUI/NIconEnums.h
    include/QtNativeUI/NInfoBar.h
    include/QtNativeUI/NLabel.h
    include/QtNativeUI/NLineEdit.h
    include/QtNativeUI/NListView.h
    include/QtNativeUI/NMainWindow.h
    include/QtNativeUI/NMenu.h
    include/QtNativeUI/NMenuBar.h
    include/QtNativeUI/NNavigationBar.h
    include/QtNativeUI/NNavigationRouter.h
    include/QtNativeUI/NNavigationView.h
    include/QtNativeUI/NOverlay.h
    include/QtNativeUI/NPageComponent.h
    include/QtNativeUI/NPivot.h
    include/QtNativeUI/NPlainTextEdit.h
    include/QtNativeUI/NProgressBar.h
    include/QtNativeUI/NProgressRing.h
    include/QtNativeUI/NPushButton.h
    include/QtNativeUI/NRadioButton.h
    include/QtNativeUI/NRangeSlider.h
    include/QtNativeUI/NScrollArea.h
    include/QtNativeUI/NScrollBar.h
    include/QtNativeUI/NSlider.h
    include/QtNativeUI/NSpinBox.h
    include/QtNativeUI/NStackedWidget.h
    include/QtNativeUI/NTabBar.h
    include/QtNativeUI/NTabWidget.h
    include/QtNativeUI/NTextEdit.h
    include/QtNativeUI/NTheme.h
    include/QtNativeUI/NToggleButton.h
    include/QtNativeUI/NToggleSwitch.h
    include/QtNativeUI/NToolButton.h
    include/QtNativeUI/NToolTip.h
    include/QtNativeUI/NTreeView.h
    include/QtNativeUI/NWindowBar.h
    include/QtNativeUI/NWindowButton.h
    include/QtNativeUI/singleton.h
    include/QtNativeUI/stdafx.h
)

set(QtNativeUI_RESOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons.qrc"
        "${CMAKE_CURRENT_SOURCE_DIR}/translations/translations.qrc"
)

# 定义翻译文件
set(TS_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/qtnativeui_zh_CN.ts
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/qtnativeui_en_US.ts
        # 添加更多语言...
)

# 设置lupdate目标
qt_add_lupdate(QtNativeUI
        TS_FILES ${TS_FILES}
        SOURCES ${QtNativeUI_SOURCES} ${QtNativeUI_HEADERS}
        INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 为每个TS文件创建一个自定义命令，直接在源目录中生成QM文件
foreach (TS_FILE ${TS_FILES})
    get_filename_component(QM_BASENAME ${TS_FILE} NAME_WE)
    set(QM_FILE "${CMAKE_CURRENT_SOURCE_DIR}/translations/${QM_BASENAME}.qm")

    add_custom_command(
            OUTPUT ${QM_FILE}
            COMMAND Qt${QT_VERSION_MAJOR}::lrelease ${TS_FILE} -qm ${QM_FILE}
            DEPENDS ${TS_FILE}
            COMMENT "Generating ${QM_BASENAME}.qm"
            VERBATIM
    )
    list(APPEND QM_FILES ${QM_FILE})
endforeach ()

# 添加一个自定义目标，确保QM文件在构建时生成
add_custom_target(translations ALL DEPENDS ${QM_FILES})

# 创建库目标
if (BUILD_SHARED_LIBS)
    qt_add_library(QtNativeUI SHARED ${QtNativeUI_SOURCES} ${QtNativeUI_HEADERS} ${QtNativeUI_RESOURCES})
    target_compile_definitions(QtNativeUI
            PRIVATE
            QTNATIVEUI_LIBRARY
            PUBLIC
            QtNativeUI_SHARED
            QT_SHARED
    )
    if (MSVC)
        set_target_properties(QtNativeUI PROPERTIES
                WINDOWS_EXPORT_ALL_SYMBOLS ON
        )
    endif ()
else ()
    qt_add_library(QtNativeUI STATIC ${QtNativeUI_SOURCES} ${QtNativeUI_HEADERS} ${QtNativeUI_RESOURCES})
endif ()

# 在创建QtNativeUI目标后添加依赖
add_dependencies(QtNativeUI translations)

set_target_properties(QtNativeUI PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)


add_library(QtNativeUI::QtNativeUI ALIAS QtNativeUI)


# 设置目标属性
target_include_directories(QtNativeUI
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/private
)

install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/include/QtNativeUI/QtNativeUI_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QtNativeUI
)

# 设置编译选项
target_compile_features(QtNativeUI PUBLIC cxx_std_17)

# 编译器特定配置
if (MSVC)
    target_compile_options(QtNativeUI
            PRIVATE
            /W4             # 警告级别
            /Zc:__cplusplus # 启用正确的 __cplusplus 宏
            /utf-8          # 使用 UTF-8 编码
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(QtNativeUI
            PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -fPIC
    )
endif ()

# 设置调试信息
target_compile_options(QtNativeUI PRIVATE
        $<$<CONFIG:Debug>:
        $<$<CXX_COMPILER_ID:MSVC>:/Zi>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-g>
        >
)

# 链接Qt库和QWindowKit
target_link_libraries(QtNativeUI
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        PRIVATE
        QWKCore
        QWKWidgets
)

# 安装配置
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# 安装头文件
install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/QtNativeUI
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 安装库文件
install(
        TARGETS QtNativeUI
        EXPORT QtNativeUITargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 导出目标
install(
        EXPORT QtNativeUITargets
        FILE QtNativeUITargets.cmake
        NAMESPACE QtNativeUI::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtNativeUI
)

# 生成版本文件
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/QtNativeUIConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

# 配置Config文件
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/QtNativeUIConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/QtNativeUIConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtNativeUI
)

# 安装配置文件
install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/QtNativeUIConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/QtNativeUIConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtNativeUI
)


if (QtNativeUI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (QtNativeUI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()