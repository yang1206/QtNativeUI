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
    
    mutable QColor _cachedTextColor;
    mutable QFont _cachedFont;
    mutable QPalette _cachedPalette;
    mutable bool _textColorCacheValid = false;
    mutable bool _fontCacheValid = false;
    mutable bool _paletteCacheValid = false;
    
    void invalidateColorCache() {
        _textColorCacheValid = false;
        _paletteCacheValid = false;
    }
    
    void invalidateFontCache() {
        _fontCacheValid = false;
    }
};

#endif // NLABEL_P_H