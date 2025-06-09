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
#define NDesignToken(key) nTheme->getToken(key)
#define NRadiusToken(key) nTheme->getRadius(key)
#define NSpacingToken(key) nTheme->getSpacing(key)
#define NFontSizeToken(key) nTheme->getFontSize(key)
#define NFontWeightToken(key) nTheme->getFontWeight(key)
#define NElevationToken(key) nTheme->getElevation(key)
#define NAnimationToken(key) nTheme->getAnimationDuration(key)
#define NAnimationEasingToken(key) nTheme->getAnimationEasing(key)
class NThemePrivate;
/**
 * @brief Theme management class for Qt applications
 *
 * NTheme provides a comprehensive theme system that supports light/dark modes,
 * accent colors, and design tokens. It can synchronize with the system theme
 * and integrates with Qt's native theme system when available.
 */
class QTNATIVEUI_EXPORT NTheme : public QObject {
    Q_OBJECT
    Q_Q_CREATE(NTheme)
    Q_SINGLETON_CREATE_H(NTheme)

    Q_PROPERTY(bool isDarkMode READ isDarkMode NOTIFY darkModeChanged)
    Q_PROPERTY(NThemeType::ThemeMode themeMode READ themeMode WRITE setThemeMode NOTIFY themeModeChanged)
    Q_PROPERTY(NAccentColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)

  public:
    bool                  isDarkMode() const;
    NThemeType::ThemeMode themeMode() const;
    /**
     * @brief Sets the theme mode
     *
     * This method changes the theme mode and updates all related components.
     * When Qt 6.5+ is available, it also synchronizes with Qt's ColorScheme.
     *
     * @param mode The theme mode to set (Light, Dark, or System)
     */
    void setThemeMode(NThemeType::ThemeMode mode);

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    /**
     * @brief Sets Qt's ColorScheme directly
     *
     * This method allows direct control of Qt's ColorScheme and synchronizes
     * NTheme's state accordingly. Available only in Qt 6.5 and later.
     *
     * @param scheme The Qt ColorScheme to set
     */
    void setQtColorScheme(Qt::ColorScheme scheme);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    /**
     * @brief Resets Qt's ColorScheme to follow the system
     *
     * This method resets Qt's ColorScheme to follow the system settings
     * and updates NTheme's state accordingly. Available only in Qt 6.8 and later.
     */
    void unsetQtColorScheme();
#endif
#endif

    /**
     * @brief Returns the current accent color
     * @return The current accent color
     */
    NAccentColor accentColor() const;
    /**
     * @brief Sets the accent color
     * @param color The accent color to set
     */
    void setAccentColor(const NAccentColor& color);
    /**
     * @brief Sets the accent color from a QColor
     *
     * Converts the QColor to an NAccentColor and sets it as the accent color.
     *
     * @param color The QColor to use as accent color
     */
    void setAccentColor(const QColor& color);

    /**
     * @brief Returns the system accent color
     *
     * On Qt 6.6+, uses QPalette::Accent.
     * On earlier versions, falls back to QPalette::Highlight or QPalette::Link.
     * If no suitable color is found, returns a default blue color.
     *
     * @return The detected system accent color
     */
    QColor getSystemAccentColor() const;

    /**
     * @brief Configures NTheme to use the system accent color
     *
     * When enabled, NTheme will monitor system changes and update
     * the accent color automatically when the system accent color changes.
     */
    void useSystemAccentColor();

    QColor getColor(NFluentColorKey::Key key) const;
    void   setColor(NFluentColorKey::Key key, const QColor& color);

    /**
     * @brief Gets a color for a specific theme mode
     *
     * This method allows retrieving colors for a theme mode different
     * from the current one without changing the global theme.
     *
     * @param key The color key to retrieve
     * @param mode The theme mode to use
     * @return The color for the specified key in the specified theme mode
     */
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

    // 设计令牌 - 通用方法
    template <typename T>
    QVariant getToken(const T& key) const;

    template <typename T>
    void setToken(const T& key, const QVariant& value);

    // 设计令牌
    QVariant getRadius(NDesignTokenKey::Radius key) const;
    QVariant getSpacing(NDesignTokenKey::Spacing key) const;
    QVariant getFontSize(NDesignTokenKey::FontSize key) const;
    QVariant getFontWeight(NDesignTokenKey::FontWeight key) const;
    QVariant getElevation(NDesignTokenKey::Elevation key) const;
    QVariant getAnimationDuration(NDesignTokenKey::AnimationDuration key) const;
    QVariant getAnimationEasing(NDesignTokenKey::AnimationEasing key) const;

    void drawEffectShadow(QPainter*                  painter,
                          QRect                      widgetRect,
                          int                        shadowBorderWidth,
                          int                        borderRadius,
                          NDesignTokenKey::Elevation elevationKey);

    // 重置为默认值
    void resetToDefaults();

  signals:
    void darkModeChanged(bool isDark);
    void themeModeChanged(NThemeType::ThemeMode mode);
    void accentColorChanged(const NAccentColor& color);
    void systemAccentColorChanged(const QColor& color);

  private:
    NTheme(QObject* parent = nullptr);
    ~NTheme();

    // 更新主题状态
    void updateThemeState();

    static NTheme* s_instance;
};
#endif // QTNATIVEUI_NTHEME_H