#ifndef NLABEL_P_H
#define NLABEL_P_H

#include <QFont>
#include <QObject>
#include "QtNativeUI/NLabel.h"
#include "QtNativeUI/NTheme.h"

class NLabelPrivate {
    Q_D_CREATE(NLabel)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextDisabledColor)
  public:
    explicit NLabelPrivate(NLabel* q);
    ~NLabelPrivate();

    void updateFont();
    void updateColor();

    // 属性
    NLabelType::Type _type   = NLabelType::Default;
    bool             _isDark = false;

    // 原始字体
    QFont _originalFont;
};

#endif // NLABEL_P_H