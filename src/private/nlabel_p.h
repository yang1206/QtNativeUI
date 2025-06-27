#ifndef NLABEL_P_H
#define NLABEL_P_H

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

    NLabelType::Type _type   = NLabelType::Default;
    bool             _isDark = false;

    QFont _originalFont;
};

#endif // NLABEL_P_H