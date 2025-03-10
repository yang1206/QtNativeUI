#ifndef QTNATIVEUI_NTHEME_H
#define QTNATIVEUI_NTHEME_H

#include <QColor>
#include <QObject>
#include "NEnums.h"
#include "stdafx.h"

/**
 * @brief 主题管理类
 * 提供对简化的Fluent Design主题系统的访问
 */
class QTNATIVEUI_EXPORT NTheme : public QObject {
    Q_OBJECT
  public:
    static NTheme* instance();

    // 获取语义颜色角色对应的颜色
    QColor color(NThemeType::ColorRole role) const;

    // 设置语义颜色角色对应的颜色
    void setColor(NThemeType::ColorRole role, const QColor& color);

    // 判断当前是否为暗色模式
    bool isDarkMode() const;

    // 设置暗色模式
    void setDarkMode(bool dark);

    // 设置主题模式
    void setThemeMode(NThemeType::ThemeMode mode);

    // 获取当前主题模式
    NThemeType::ThemeMode themeMode() const;

    // 重置为默认颜色
    void resetToDefaults();

  signals:
    void colorChanged(NThemeType::ColorRole role, const QColor& color);
    void darkModeChanged(bool isDark);
    void themeModeChanged(NThemeType::ThemeMode mode);

  private:
    NTheme(QObject* parent = nullptr);
    ~NTheme();

    // 使用宏创建d指针
    class NThemePrivate;
    N_DECLARE_PRIVATE(NTheme)

    static NTheme* s_instance;
};

#endif // QTNATIVEUI_NTHEME_H