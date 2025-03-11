#!/usr/bin/env node

const fs = require('fs');
const path = require('path');
const { DOMParser } = require('xmldom');
const xpath = require('xpath');

// 解析命令行参数
if (process.argv.length < 4) {
  console.error('Usage: node color_generator.js <xaml_file> <output_file>');
  process.exit(1);
}

const xamlFile = process.argv[2];
const outputFile = process.argv[3];

// 解析XAML颜色字符串为QColor格式字符串
function parseColor(xamlColor) {
  if (!xamlColor) return 'QColor()';
  
  // 处理 #RRGGBB 格式
  if (xamlColor.length === 7 && xamlColor[0] === '#') {
    return `QColor("${xamlColor}")`;
  }
  
  // 处理 #AARRGGBB 格式
  if (xamlColor.length === 9 && xamlColor[0] === '#') {
    const alphaHex = xamlColor.substring(1, 3);
    const rgbHex = xamlColor.substring(3);
    const alpha = parseInt(alphaHex, 16);
    
    // 使用辅助函数创建带有alpha的颜色
    return `colorWithAlpha("#${rgbHex}", ${alpha})`;
  }
  
  // 处理其他格式或无效格式
  return 'QColor()';
}

// 解析XAML文件
function parseXaml(xamlFile) {
  const xamlContent = fs.readFileSync(xamlFile, 'utf-8');
  const doc = new DOMParser().parseFromString(xamlContent, 'application/xml');
  
  const darkColors = {};
  const lightColors = {};
  
  // 查找ResourceDictionary节点
  const select = xpath.useNamespaces({
    'x': 'http://schemas.microsoft.com/winfx/2006/xaml',
    'default': 'http://schemas.microsoft.com/winfx/2006/xaml/presentation'
  });
  
  // 查找默认主题（暗色）的颜色
  const defaultDict = select("//default:ResourceDictionary[@x:Key='Default']", doc)[0];
  if (defaultDict) {
    const colorNodes = select(".//default:Color", defaultDict);
    colorNodes.forEach(node => {
      const key = node.getAttribute('x:Key');
      const value = node.textContent.trim();
      darkColors[key] = parseColor(value);
    });
  }
  
  // 查找亮色主题的颜色
  const lightDict = select("//default:ResourceDictionary[@x:Key='Light']", doc)[0];
  if (lightDict) {
    const colorNodes = select(".//default:Color", lightDict);
    colorNodes.forEach(node => {
      const key = node.getAttribute('x:Key');
      const value = node.textContent.trim();
      lightColors[key] = parseColor(value);
    });
  }
  
  return { darkColors, lightColors };
}

// 生成头文件
function generateHeader(darkColors, lightColors, outputFile) {
  let content = `// 自动生成的文件，请勿手动修改
// 从Microsoft Fluent Design颜色资源生成

#ifndef NFLUENTCOLORS_H
#define NFLUENTCOLORS_H

#include <QColor>
#include <QString>
#include <QMap>

// 辅助函数：创建带有alpha值的颜色
inline QColor colorWithAlpha(const QString& hex, int alpha) {
    QColor color(hex);
    color.setAlpha(alpha);
    return color;
}

// 暗色主题颜色
inline const QMap<QString, QColor> DarkThemeColors = {
`;

  // 添加暗色主题颜色
  for (const [key, value] of Object.entries(darkColors)) {
    content += `    {"${key}", ${value}},\n`;
  }
  
  content += `};

// 亮色主题颜色
inline const QMap<QString, QColor> LightThemeColors = {
`;

  // 添加亮色主题颜色
  for (const [key, value] of Object.entries(lightColors)) {
    content += `    {"${key}", ${value}},\n`;
  }
  
  content += `};

// 颜色常量
class NFluentColorConstants {
public:
`;

  // 生成颜色常量声明
  const allKeys = new Set([...Object.keys(darkColors), ...Object.keys(lightColors)]);
  for (const key of allKeys) {
    content += `    inline static const QString ${key};\n`;
  }
  
  content += `};

`;

  // 生成颜色常量定义
  for (const key of allKeys) {
    content += `inline const QString NFluentColorConstants::${key} = "${key}";\n`;
  }
  
  content += `
#endif // NFLUENTCOLORS_H
`;

  fs.writeFileSync(outputFile, content);
}

// 主函数
try {
  const { darkColors, lightColors } = parseXaml(xamlFile);
  generateHeader(darkColors, lightColors, outputFile);
  
  console.log(`Successfully generated ${outputFile}`);
  console.log(`Dark colors: ${Object.keys(darkColors).length}`);
  console.log(`Light colors: ${Object.keys(lightColors).length}`);
} catch (error) {
  console.error('Error:', error.message);
  process.exit(1);
} 