#!/usr/bin/env python3

import sys
import re
import xml.etree.ElementTree as ET
import os

def parse_color(xaml_color):
    """解析XAML颜色为QColor格式字符串"""
    if not xaml_color:
        return "QColor()"
    
    # 处理 #RRGGBB 格式
    if len(xaml_color) == 7 and xaml_color[0] == '#':
        return f'QColor("{xaml_color}")'
    
    # 处理 #AARRGGBB 格式
    if len(xaml_color) == 9 and xaml_color[0] == '#':
        alpha_hex = xaml_color[1:3]
        rgb_hex = xaml_color[3:]
        alpha = int(alpha_hex, 16)
        # 使用辅助函数创建带有alpha的颜色
        return f'colorWithAlpha("#{rgb_hex}", {alpha})'
    
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
    # 由于ElementTree的XPath支持有限，我们使用循环查找
    for resource_dict in root.findall(".//{http://schemas.microsoft.com/winfx/2006/xaml/presentation}ResourceDictionary"):
        key_attr = resource_dict.get("{http://schemas.microsoft.com/winfx/2006/xaml}Key")
        
        if key_attr == "Default":
            # 解析暗色主题颜色
            for color_elem in resource_dict.findall(".//{http://schemas.microsoft.com/winfx/2006/xaml/presentation}Color"):
                color_key = color_elem.get("{http://schemas.microsoft.com/winfx/2006/xaml}Key")
                color_value = color_elem.text.strip() if color_elem.text else ""
                dark_colors[color_key] = parse_color(color_value)
        
        elif key_attr == "Light":
            # 解析亮色主题颜色
            for color_elem in resource_dict.findall(".//{http://schemas.microsoft.com/winfx/2006/xaml/presentation}Color"):
                color_key = color_elem.get("{http://schemas.microsoft.com/winfx/2006/xaml}Key")
                color_value = color_elem.text.strip() if color_elem.text else ""
                light_colors[color_key] = parse_color(color_value)
    
    return dark_colors, light_colors

def generate_header(dark_colors, light_colors, output_file):
    """生成C++头文件"""
    with open(output_file, 'w') as f:
        f.write("// 自动生成的文件，请勿手动修改\n")
        f.write("// 从Microsoft Fluent Design颜色资源生成\n\n")
        f.write("#ifndef NFLUENTCOLORS_H\n")
        f.write("#define NFLUENTCOLORS_H\n\n")
        f.write("#include <QColor>\n")
        f.write("#include <QString>\n")
        f.write("#include <QMap>\n\n")
        
        # 添加辅助函数
        f.write("// 辅助函数：创建带有alpha值的颜色\n")
        f.write("inline QColor colorWithAlpha(const QString& hex, int alpha) {\n")
        f.write("    QColor color(hex);\n")
        f.write("    color.setAlpha(alpha);\n")
        f.write("    return color;\n")
        f.write("}\n\n")
        
        # 生成暗色主题颜色
        f.write("// 暗色主题颜色\n")
        f.write("static const QMap<QString, QColor> DarkThemeColors = {\n")
        for key, value in dark_colors.items():
            f.write(f"    {{\"{key}\", {value}}},\n")
        f.write("};\n\n")
        
        # 生成亮色主题颜色
        f.write("// 亮色主题颜色\n")
        f.write("static const QMap<QString, QColor> LightThemeColors = {\n")
        for key, value in light_colors.items():
            f.write(f"    {{\"{key}\", {value}}},\n")
        f.write("};\n\n")
        
        # 生成颜色常量类
        f.write("// 颜色常量\n")
        f.write("class NFluentColorConstants {\n")
        f.write("public:\n")
        
        # 收集所有唯一的键
        all_keys = set(list(dark_colors.keys()) + list(light_colors.keys()))
        
        # 生成常量声明
        for key in sorted(all_keys):
            # 将键转换为有效的C++标识符
            cpp_key = key
            f.write(f"    static const QString {cpp_key};\n")
        
        f.write("};\n\n")
        
        # 生成常量定义
        for key in sorted(all_keys):
            cpp_key = key
            f.write(f'inline const QString NFluentColorConstants::{cpp_key} = "{key}";\n')
        
        f.write("\n#endif // NFLUENTCOLORS_H\n")

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
        print("Usage: python color_generator.py <output_file> [xaml_file]")
        print("If xaml_file is not provided, it will be downloaded from Microsoft's GitHub repository")
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
    
    # 生成头文件
    print(f"Generating header file: {output_file}")
    generate_header(dark_colors, light_colors, output_file)
    
    print(f"Successfully generated {output_file}")
    print(f"Dark colors: {len(dark_colors)}")
    print(f"Light colors: {len(light_colors)}")

if __name__ == "__main__":
    main() 