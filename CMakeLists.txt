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
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 收集源文件 - 修改方式收集所有需要的文件
file(GLOB_RECURSE QtNativeUI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

file(GLOB_RECURSE QtNativeUI_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/QtNativeUI/*.h"
)

set(QtNativeUI_RESOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons.qrc"
)


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
    qt_add_library(QtNativeUI STATIC ${QtNativeUI_SOURCES} ${QtNativeUI_HEADERS}
    )
endif ()

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
target_compile_features(QtNativeUI PUBLIC cxx_std_20)

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

# 链接Qt库
target_link_libraries(QtNativeUI
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
)

if (WIN32)
    target_link_libraries(QtNativeUI PRIVATE dwmapi uxtheme)
endif ()

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

# 构建示例
if (QtNativeUI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()