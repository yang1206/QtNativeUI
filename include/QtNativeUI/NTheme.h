#ifndef QTNATIVEUI_NTHEME_H
#define QTNATIVEUI_NTHEME_H

#include <QtNativeUI/QtNativeUI_export.h>
#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NMacros.h>
#include <QObject>
#include <QColor>

namespace QtNativeUI {

/**
 * @brief 主题管理类
 * 提供对简化的Fluent Design主题系统的访问
 */
class QTNATIVEUI_EXPORT NTheme : public QObject
{
    Q_OBJECT
public:
    static NTheme* instance();
    
    // 获取语义颜色角色对应的颜色
    QColor color(NColorRole role) const;
    
    // 设置语义颜色角色对应的颜色
    void setColor(NColorRole role, const QColor& color);
    
    // 判断当前是否为暗色模式
    bool isDarkMode() const;
    
    // 设置暗色模式
    void setDarkMode(bool dark);
    
    // 设置主题模式
    void setThemeMode(NThemeMode mode);
    
    // 获取当前主题模式
    NThemeMode themeMode() const;
    
    // 重置为默认颜色
    void resetToDefaults();

signals:
    void colorChanged(NColorRole role, const QColor& color);
    void darkModeChanged(bool isDark);
    void themeModeChanged(NThemeMode mode);

private:
    NTheme(QObject* parent = nullptr);
    ~NTheme();
    
    // 使用宏创建d指针
    class NThemePrivate;
    N_DECLARE_PRIVATE(NTheme)
    
    static NTheme* s_instance;
};

} // namespace QtNativeUI

#endif // QTNATIVEUI_NTHEME_H 