#ifndef QTNATIVEUI_NTHEME_H
#define QTNATIVEUI_NTHEME_H

#include <QPainter>
#include "NColor.h"
#include "NEnums.h"
#include "NFluentColors.h"
#include "singleton.h"
#include "stdafx.h"

#define nTheme NTheme::getInstance()
#define NThemeColor(key, themeMode) nTheme->getColorForTheme(key, themeMode)
#define NAccentColor(type) getAccentColor(type)
// 使用宏创建d指针
class NThemePrivate;
/**
 * @brief 主题管理类
 */
class QTNATIVEUI_EXPORT NTheme : public QObject {
    Q_OBJECT
    Q_Q_CREATE(NTheme)
    Q_SINGLETON_CREATE_H(NTheme)

    Q_PROPERTY(bool isDarkMode READ isDarkMode NOTIFY darkModeChanged)
    Q_PROPERTY(NThemeType::ThemeMode themeMode READ themeMode WRITE setThemeMode NOTIFY themeModeChanged)
    Q_PROPERTY(NAccentColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)

  public:
    // 获取/设置主题模式
    bool                  isDarkMode() const;
    NThemeType::ThemeMode themeMode() const;
    void                  setThemeMode(NThemeType::ThemeMode mode);

    // 强调色
    NAccentColor accentColor() const;
    void         setAccentColor(const NAccentColor& color);
    void         setAccentColor(const QColor& color);

    // 颜色获取与设置 - 类型安全版本
    QColor getColor(NFluentColorKey::Key key) const;
    void   setColor(NFluentColorKey::Key key, const QColor& color);

    QColor getColorForTheme(NFluentColorKey::Key key, NThemeType::ThemeMode mode) const;

    // 获取预定义的强调色
    NAccentColor getAccentColor(NAccentColorType::Type type) const;

    // 根据当前主题获取预定义强调色的适当变体
    QColor getThemedAccentColor(NAccentColorType::Type type) const;

    // 获取强调色的特定变体
    QColor getAccentColorVariant(NAccentColorType::Type type, const QString& variant) const;

    // 获取所有可用的颜色键
    QList<NFluentColorKey::Key> getAllColorKeys() const;

    // 获取所有当前主题颜色
    QMap<NFluentColorKey::Key, QColor> getAllColors() const;

    // 设计令牌
    QVariant getToken(const QString& key) const;
    void     setToken(const QString& key, const QVariant& value);

    void drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius);

    // 重置为默认值
    void resetToDefaults();

  signals:
    void darkModeChanged(bool isDark);
    void themeModeChanged(NThemeType::ThemeMode mode);
    void accentColorChanged(const NAccentColor& color);

  private:
    NTheme(QObject* parent = nullptr);
    ~NTheme();

    // 系统主题检测
    bool detectSystemTheme() const;

    // 更新主题状态
    void updateThemeState();

    static NTheme* s_instance;
};
#endif // QTNATIVEUI_NTHEME_H