#include "nlabel_p.h"
#include <QApplication>
#include <QFont>
#include "QtNativeUI/NTheme.h"

NLabelPrivate::NLabelPrivate(NLabel* q) : q_ptr(q) {
    _isDark = nTheme->isDarkMode();

    // 初始化颜色
    _pLightTextColor         = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    _pDarkTextColor          = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    _pLightTextDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    _pDarkTextDisabledColor  = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);
}

NLabelPrivate::~NLabelPrivate() {}

void NLabelPrivate::updateFont() {
    Q_Q(NLabel);

    QFont font = _originalFont;

    // 字体大小
    int fontSize = 0;
    // 字重
    int fontWeight = 0;

    // 根据类型设置字体
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
            // 保留原始字体
            break;
    }

    if (_type != NLabelType::Default) {
        font.setPointSize(fontSize);
        font.setWeight(static_cast<QFont::Weight>(fontWeight));
    }

    q->setFont(font);
}

void NLabelPrivate::updateColor() {
    Q_Q(NLabel);

    QColor textColor;

    if (q->isEnabled()) {
        textColor = _isDark ? _pDarkTextColor : _pLightTextColor;
    } else {
        textColor = _isDark ? _pDarkTextDisabledColor : _pLightTextDisabledColor;
    }

    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, textColor);
    palette.setColor(QPalette::Text, textColor);
    q->setPalette(palette);
}