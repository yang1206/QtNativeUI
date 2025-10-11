#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import re
import xml.etree.ElementTree as ET
import os
import json

def parse_color(xaml_color):
    """解析XAML颜色为QColor格式字符串"""
    if not xaml_color:
        return "QColor()"
    
    # 将十六进制颜色转换为整数构造函数调用
    if xaml_color[0] == '#':
        # 移除 # 前缀
        hex_color = xaml_color[1:]
        
        # 处理不同格式
        if len(hex_color) == 8:  # #AARRGGBB
            a = int(hex_color[0:2], 16)
            r = int(hex_color[2:4], 16)
            g = int(hex_color[4:6], 16)
            b = int(hex_color[6:8], 16)
            return f'QColor({r}, {g}, {b}, {a})'
        elif len(hex_color) == 6:  # #RRGGBB
            r = int(hex_color[0:2], 16)
            g = int(hex_color[2:4], 16)
            b = int(hex_color[4:6], 16)
            return f'QColor({r}, {g}, {b})'
        elif len(hex_color) == 4:  # #ARGB
            a = int(hex_color[0] * 2, 16)
            r = int(hex_color[1] * 2, 16)
            g = int(hex_color[2] * 2, 16)
            b = int(hex_color[3] * 2, 16)
            return f'QColor({r}, {g}, {b}, {a})'
        elif len(hex_color) == 3:  # #RGB
            r = int(hex_color[0] * 2, 16)
            g = int(hex_color[1] * 2, 16)
            b = int(hex_color[2] * 2, 16)
            return f'QColor({r}, {g}, {b})'
    
    return "QColor()"

def parse_xaml(xaml_file):
    """解析XAML文件中的颜色定义"""
    # 注册命名空间
    ET.register_namespace('', "http://schemas.microsoft.com/winfx/2006/xaml/presentation")
    ET.register_namespace('x', "http://schemas.microsoft.com/winfx/2006/xaml")
    
    # 解析XML文件
    tree = ET.parse(xaml_file)
    root = tree.getroot()
    
    # 查找ResourceDictionary节点
    dark_colors = {}
    light_colors = {}
    
    # 使用XPath查找带有特定x:Key属性的ResourceDictionary
    for resource_dict in root.findall(".//{http://schemas.microsoft.com/winfx/2006/xaml/presentation}ResourceDictionary"):
        key_attr = resource_dict.get("{http://schemas.microsoft.com/winfx/2006/xaml}Key")
        
        if key_attr == "Default":
            # 解析暗色主题颜色
            # 1. 处理 Color 标签
            for color_elem in resource_dict.findall(".//{http://schemas.microsoft.com/winfx/2006/xaml/presentation}Color"):
                color_key = color_elem.get("{http://schemas.microsoft.com/winfx/2006/xaml}Key")
                color_value = color_elem.text.strip() if color_elem.text else ""
                dark_colors[color_key] = parse_color(color_value)
            
            # 2. 处理 SolidColorBrush 标签
            for brush_elem in resource_dict.findall(".//{http://schemas.microsoft.com/winfx/2006/xaml/presentation}SolidColorBrush"):
                brush_key = brush_elem.get("{http://schemas.microsoft.com/winfx/2006/xaml}Key")
                if brush_key and brush_key.startswith("SystemColor") and brush_key.endswith("ColorBrush"):
                    # 提取颜色值
                    color_value = brush_elem.get("Color")
                    if color_value:
                        # 移除 "Brush" 后缀，只保留颜色键名
                        color_key = brush_key[:-5]  # 去掉 "Brush" 后缀
                        dark_colors[color_key] = parse_color(color_value)
        
        elif key_attr == "Light":
            # 解析亮色主题颜色
            # 1. 处理 Color 标签
            for color_elem in resource_dict.findall(".//{http://schemas.microsoft.com/winfx/2006/xaml/presentation}Color"):
                color_key = color_elem.get("{http://schemas.microsoft.com/winfx/2006/xaml}Key")
                color_value = color_elem.text.strip() if color_elem.text else ""
                light_colors[color_key] = parse_color(color_value)
            
            # 2. 处理 SolidColorBrush 标签
            for brush_elem in resource_dict.findall(".//{http://schemas.microsoft.com/winfx/2006/xaml/presentation}SolidColorBrush"):
                brush_key = brush_elem.get("{http://schemas.microsoft.com/winfx/2006/xaml}Key")
                if brush_key and brush_key.startswith("SystemColor") and brush_key.endswith("ColorBrush"):
                    # 提取颜色值
                    color_value = brush_elem.get("Color")
                    if color_value:
                        # 移除 "Brush" 后缀，只保留颜色键名
                        color_key = brush_key[:-5]  # 去掉 "Brush" 后缀
                        light_colors[color_key] = parse_color(color_value)
    
    return dark_colors, light_colors

def load_custom_colors(custom_colors_file):
    """加载自定义颜色配置文件"""
    if not os.path.exists(custom_colors_file):
        return {}
    
    try:
        with open(custom_colors_file, 'r', encoding='utf-8') as f:
            config = json.load(f)
        
        custom_colors = config.get('custom_colors', {})
        dark_colors = {}
        light_colors = {}
        
        for color_name, color_data in custom_colors.items():
            if 'dark' in color_data:
                dark_colors[color_name] = color_data['dark']
            if 'light' in color_data:
                light_colors[color_name] = color_data['light']
        
        return dark_colors, light_colors
    except Exception as e:
        print(f"Error loading custom colors: {e}")
        return {}, {}

def generate_header(dark_colors, light_colors, output_file, custom_dark_colors={}, custom_light_colors={}):
    """生成C++头文件"""
    # 收集所有唯一的键，并排序以生成一致的枚举
    all_keys = sorted(set(list(dark_colors.keys()) + list(light_colors.keys()) +
                         list(custom_dark_colors.keys()) + list(custom_light_colors.keys())))
    
    # 使用UTF-8编码打开文件
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("""// WARNING: This file is automatically generated.
// DO NOT EDIT IT MANUALLY!
//
// Generated from Microsoft's Fluent UI XAML Theme Resources
// Source: https://github.com/microsoft/microsoft-ui-xaml
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

""")
        f.write("#ifndef NFLUENTCOLORS_H\n")
        f.write("#define NFLUENTCOLORS_H\n\n")
        f.write("#include <QColor>\n")
        f.write("#include <QString>\n")
        f.write("#include <QMap>\n")
        f.write("#include <QMetaEnum>\n\n")
        
        # 添加辅助函数
        f.write("// 辅助函数：创建带有alpha值的颜色（alpha为0-255）\n")
        f.write("inline QColor colorWithAlpha(int r, int g, int b, int alpha) {\n")
        f.write("    return QColor(r, g, b, alpha);\n")
        f.write("}\n\n")
        
        f.write("// 辅助函数：从现有颜色创建带有alpha值的颜色\n")
        f.write("inline QColor colorWithAlpha(const QColor& baseColor, int alpha) {\n")
        f.write("    QColor color = baseColor;\n")
        f.write("    color.setAlpha(alpha);\n")
        f.write("    return color;\n")
        f.write("}\n\n")
        
        # 添加新的百分比alpha函数
        f.write("// 辅助函数：创建带有百分比alpha值的颜色（alphaPercent为0-100）\n")
        f.write("inline QColor colorWithAlphaPercent(int r, int g, int b, int alphaPercent) {\n")
        f.write("    int alpha = static_cast<int>(255.0 * alphaPercent / 100.0);\n")
        f.write("    return QColor(r, g, b, alpha);\n")
        f.write("}\n\n")
        
        f.write("// 辅助函数：从现有颜色创建带有百分比alpha值的颜色\n")
        f.write("inline QColor colorWithAlphaPercent(const QColor& baseColor, int alphaPercent) {\n")
        f.write("    QColor color = baseColor;\n")
        f.write("    int alpha = static_cast<int>(255.0 * alphaPercent / 100.0);\n")
        f.write("    color.setAlpha(alpha);\n")
        f.write("    return color;\n")
        f.write("}\n\n")
        
        # 生成枚举
        f.write("// Fluent Design 颜色枚举 - 自动生成\n")
        f.write("namespace NFluentColorKey {\n")
        f.write("    enum Key {\n")
        for key in all_keys:
            f.write(f"        {key},\n")
        f.write("        Count  // 用于计数和边界检查\n")
        f.write("    };\n")
        f.write("}\n\n")
        
        # 添加枚举到字符串的转换函数
        f.write("// 枚举到字符串的转换函数\n")
        f.write("inline QString fluentColorKeyToString(NFluentColorKey::Key key) {\n")
        f.write("    switch (key) {\n")
        for key in all_keys:
            f.write(f"        case NFluentColorKey::{key}:\n")
            f.write(f"            return \"{key}\";\n")
        f.write("        case NFluentColorKey::Count:\n")
        f.write("            return \"\";\n")
        f.write("    }\n")
        f.write("    return \"\";\n")
        f.write("}\n\n")
        
        # 添加字符串到枚举的转换函数
        f.write("// 字符串到枚举的转换函数\n")
        f.write("inline NFluentColorKey::Key stringToFluentColorKey(const QString& keyString) {\n")
        for key in all_keys:
            f.write(f"    if (keyString == \"{key}\") return NFluentColorKey::{key};\n")
        f.write("    return NFluentColorKey::Count; // 无效的键\n")
        f.write("}\n\n")
        
        # 生成使用枚举索引的颜色映射
        f.write("// 暗色主题颜色\n")
        f.write("static const QMap<NFluentColorKey::Key, QColor> DarkThemeColors = {\n")
        
        # 首先添加官方颜色
        for key, value in dark_colors.items():
            f.write(f"    {{NFluentColorKey::{key}, {value}}},\n")
        
        # 然后添加自定义颜色
        for key, value in custom_dark_colors.items():
            f.write(f"    {{NFluentColorKey::{key}, {value}}},\n")
        
        f.write("};\n\n")
        
        f.write("// 亮色主题颜色\n")
        f.write("static const QMap<NFluentColorKey::Key, QColor> LightThemeColors = {\n")
        
        # 首先添加官方颜色
        for key, value in light_colors.items():
            f.write(f"    {{NFluentColorKey::{key}, {value}}},\n")
        
        # 然后添加自定义颜色
        for key, value in custom_light_colors.items():
            f.write(f"    {{NFluentColorKey::{key}, {value}}},\n")
        
        f.write("};\n\n")
        
        f.write("#endif // NFLUENTCOLORS_H\n")

def download_xaml(url, output_file):
    """下载XAML文件"""
    import urllib.request
    
    print(f"Downloading XAML file from {url}...")
    try:
        urllib.request.urlretrieve(url, output_file)
        print(f"Downloaded to {output_file}")
        return True
    except Exception as e:
        print(f"Error downloading file: {e}")
        return False

def main():
    # 检查命令行参数
    if len(sys.argv) < 2:
        print("Usage: python color_generator.py <output_file> [xaml_file] [custom_colors_file]")
        print("If xaml_file is not provided, it will be downloaded from Microsoft's GitHub repository")
        print("If custom_colors_file is not provided, will look for custom_colors.json in the same directory")
        sys.exit(1)
    
    output_file = sys.argv[1]
    
    # 如果提供了XAML文件路径，使用它；否则下载
    if len(sys.argv) >= 3:
        xaml_file = sys.argv[2]
    else:
        # 创建临时目录
        temp_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "temp")
        os.makedirs(temp_dir, exist_ok=True)
        
        xaml_file = os.path.join(temp_dir, "Common_themeresources_any.xaml")
        xaml_url = "https://raw.githubusercontent.com/microsoft/microsoft-ui-xaml/main/src/controls/dev/CommonStyles/Common_themeresources_any.xaml"
        
        if not os.path.exists(xaml_file) or "--force-download" in sys.argv:
            if not download_xaml(xaml_url, xaml_file):
                sys.exit(1)
    
    # 解析XAML文件
    print(f"Parsing XAML file: {xaml_file}")
    dark_colors, light_colors = parse_xaml(xaml_file)
    
    # 加载自定义颜色
    custom_colors_file = sys.argv[3] if len(sys.argv) >= 4 else os.path.join(os.path.dirname(os.path.abspath(__file__)), "custom_colors.json")
    print(f"Loading custom colors from: {custom_colors_file}")
    custom_dark_colors, custom_light_colors = load_custom_colors(custom_colors_file)
    
    # 生成头文件
    print(f"Generating header file: {output_file}")
    generate_header(dark_colors, light_colors, output_file, custom_dark_colors, custom_light_colors)
    
    print(f"Successfully generated {output_file}")
    print(f"Dark colors: {len(dark_colors)}")
    print(f"Light colors: {len(light_colors)}")
    print(f"Custom dark colors: {len(custom_dark_colors)}")
    print(f"Custom light colors: {len(custom_light_colors)}")

if __name__ == "__main__":
    main()