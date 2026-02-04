#ifndef QTNATIVEUI_NGROUPBOX_P_H
#define QTNATIVEUI_NGROUPBOX_P_H

#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NGroupBox.h>

#include "QtNativeUI/NCheckBox.h"

class QVBoxLayout;

class NGroupBoxPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightTitleColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTitleColor)
    Q_PROPERTY_CREATE_D(int, TitleSpacing)
    Q_PROPERTY_CREATE_D(bool, ShowBorder)
    Q_PROPERTY_CREATE_D(int, ContentMargin)
    Q_PROPERTY_CREATE_D(int, TitleHeight)

  public:
    explicit NGroupBoxPrivate(QObject* parent = nullptr);
    ~NGroupBoxPrivate();
    Q_D_CREATE(NGroupBox)

  private:
    NThemeType::ThemeMode    _themeMode;
    bool                     _isDark;
    NGroupBox::GroupBoxStyle _groupBoxStyle{NGroupBox::Standard};

    QString       _title;
    Qt::Alignment _alignment{Qt::AlignLeft};
    bool          _flat{false};
    bool          _checkable{false};
    bool          _checked{false};

    struct TitleIconInfo {
        bool    isRegular = true;
        quint32 iconCode  = 0;
        int     size      = 16;
        bool    hasIcon   = false;
    };
    TitleIconInfo _titleIcon;

    NCheckBox* _checkBox{nullptr};

    int _borderWidth{1};

    QColor getCurrentColor(const QColor& lightColor, const QColor& darkColor) const {
        return _isDark ? darkColor : lightColor;
    }

    QRect calculateTitleRect(const QRect& rect) const;
    QRect calculateContentRect(const QRect& rect) const;
};

#endif // QTNATIVEUI_NGROUPBOX_P_H