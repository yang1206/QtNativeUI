#include "nlabel_p.h"
#include <QApplication>
#include <QFont>
#include "QtNativeUI/NTheme.h"

NLabelPrivate::NLabelPrivate(NLabel* q) : q_ptr(q) {
    _isDark = nTheme->isDarkMode();

    _pLightTextColor         = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    _pDarkTextColor          = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    _pLightTextDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    _pDarkTextDisabledColor  = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);
}

NLabelPrivate::~NLabelPrivate() {}

void NLabelPrivate::updateFont() {
    Q_Q(NLabel);

    QFont font;
    if (!_fontCacheValid) {
        font = _originalFont;

        int fontSize = 0;
        int fontWeight = 0;

        switch (_type) {
            case NLabelType::Caption:
                fontSize   = NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt();
                fontWeight = NFontWeightToken(NDesignTokenKey::FontWeightRegular).toInt();
                break;
            case NLabelType::Body:
                fontSize   = NFontSizeToken(NDesignTokenKey::FontSizeBody).toInt();
                fontWeight = NFontWeightToken(NDesignTokenKey::FontWeightRegular).toInt();
                break;
            case NLabelType::BodyStrong:
                fontSize   = NFontSizeToken(NDesignTokenKey::FontSizeBody).toInt();
                fontWeight = NFontWeightToken(NDesignTokenKey::FontWeightSemibold).toInt();
                break;
            case NLabelType::BodyLarge:
                fontSize   = NFontSizeToken(NDesignTokenKey::FontSizeBodyLarge).toInt();
                fontWeight = NFontWeightToken(NDesignTokenKey::FontWeightRegular).toInt();
                break;
            case NLabelType::BodyStrongLarge:
                fontSize   = NFontSizeToken(NDesignTokenKey::FontSizeBodyLarge).toInt();
                fontWeight = NFontWeightToken(NDesignTokenKey::FontWeightSemibold).toInt();
                break;
            case NLabelType::Subtitle:
                fontSize   = NFontSizeToken(NDesignTokenKey::FontSizeSubTitle).toInt();
                fontWeight = NFontWeightToken(NDesignTokenKey::FontWeightSemibold).toInt();
                break;
            case NLabelType::Title:
                fontSize   = NFontSizeToken(NDesignTokenKey::FontSizeTitle).toInt();
                fontWeight = NFontWeightToken(NDesignTokenKey::FontWeightSemibold).toInt();
                break;
            case NLabelType::TitleLarge:
                fontSize   = NFontSizeToken(NDesignTokenKey::FontSizeTitleLarge).toInt();
                fontWeight = NFontWeightToken(NDesignTokenKey::FontWeightSemibold).toInt();
                break;
            case NLabelType::Display:
                fontSize   = NFontSizeToken(NDesignTokenKey::FontSizeDisplay).toInt();
                fontWeight = NFontWeightToken(NDesignTokenKey::FontWeightSemibold).toInt();
                break;
            case NLabelType::Default:
            default:
                break;
        }

        if (_type != NLabelType::Default) {
            font.setPixelSize(fontSize);
            font.setWeight(static_cast<QFont::Weight>(fontWeight));
        }

        _cachedFont = font;
        _fontCacheValid = true;
    } else {
        font = _cachedFont;
    }

    q->setFont(font);
}

void NLabelPrivate::updateColor() {
    Q_Q(NLabel);

    QColor textColor;
    if (!_textColorCacheValid) {
        if (q->isEnabled()) {
            textColor = _isDark ? _pDarkTextColor : _pLightTextColor;
        } else {
            textColor = _isDark ? _pDarkTextDisabledColor : _pLightTextDisabledColor;
        }
        _cachedTextColor = textColor;
        _textColorCacheValid = true;
    } else {
        textColor = _cachedTextColor;
    }

    QPalette palette;
    if (!_paletteCacheValid) {
        palette = q->palette();
        palette.setColor(QPalette::WindowText, textColor);
        palette.setColor(QPalette::Text, textColor);
        _cachedPalette = palette;
        _paletteCacheValid = true;
    } else {
        palette = _cachedPalette;
        palette.setColor(QPalette::WindowText, textColor);
        palette.setColor(QPalette::Text, textColor);
    }
    
    q->setPalette(palette);
}