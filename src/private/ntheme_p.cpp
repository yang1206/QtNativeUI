#include "ntheme_p.h"
#include <QApplication>
#include <QPalette>
#include <QScreen>
#include <QStyle>
#include <QVariant>

// 初始化私有实现
NTheme::NThemePrivate::NThemePrivate(NTheme* q)
    : _themeMode(NTheme::ThemeMode::System), _isDark(false), _accentColor(NColors::blue), q_ptr(q) {
    // 初始化颜色和设计令牌
    initLightColors();
    initDarkColors();
    initDesignTokens();

    // 设置系统主题检测
    setupSystemThemeDetection();
    updateDarkModeState();
}

NTheme::NThemePrivate::~NThemePrivate() {}

// 初始化设计令牌 - 基于 Windows 11 Fluent UI 规范
void NTheme::NThemePrivate::initDesignTokens() {
    // 圆角大小 (CornerRadius)
    _designTokens["cornerRadiusNone"]     = 0.0;
    _designTokens["cornerRadiusSmall"]    = 2.0;
    _designTokens["cornerRadiusDefault"]  = 4.0;
    _designTokens["cornerRadiusMedium"]   = 8.0;
    _designTokens["cornerRadiusLarge"]    = 12.0;
    _designTokens["cornerRadiusCircular"] = 999.0; // 实际应用时需计算为宽度的50%

    // 间距 (Spacing)
    _designTokens["spacingNone"] = 0;
    _designTokens["spacingXS"]   = 2;
    _designTokens["spacingS"]    = 4;
    _designTokens["spacingM"]    = 8;
    _designTokens["spacingL"]    = 12;
    _designTokens["spacingXL"]   = 16;
    _designTokens["spacingXXL"]  = 20;
    _designTokens["spacingXXXL"] = 24;

    // 字体大小 (FontSize)
    _designTokens["fontSizeCaption"]     = 12;
    _designTokens["fontSizeBody"]        = 14;
    _designTokens["fontSizeBodyLarge"]   = 16;
    _designTokens["fontSizeTitle"]       = 18;
    _designTokens["fontSizeTitleLarge"]  = 20;
    _designTokens["fontSizeHeader"]      = 24;
    _designTokens["fontSizeHeaderLarge"] = 28;

    // 字重 (FontWeight)
    _designTokens["fontWeightRegular"]  = 400;
    _designTokens["fontWeightMedium"]   = 500;
    _designTokens["fontWeightSemibold"] = 600;
    _designTokens["fontWeightBold"]     = 700;

    // 阴影层级 (Elevation) - 存储为CSS样式字符串，便于应用
    _designTokens["elevationNone"]   = QString();
    _designTokens["elevationRest"]   = QString("0 2px 4px rgba(0,0,0,0.12)");
    _designTokens["elevationHover"]  = QString("0 4px 8px rgba(0,0,0,0.14)");
    _designTokens["elevationFlyout"] = QString("0 8px 16px rgba(0,0,0,0.16)");
    _designTokens["elevationDialog"] = QString("0 16px 32px rgba(0,0,0,0.20)");

    // 动效时长 (AnimationDuration)
    _designTokens["animationFast"]     = 100;
    _designTokens["animationNormal"]   = 200;
    _designTokens["animationSlow"]     = 400;
    _designTokens["animationVerySlow"] = 600;

    // 缓动曲线 (EasingCurve)
    _designTokens["easingStandard"]   = QString("cubic-bezier(0.8, 0, 0.2, 1)"); // 标准
    _designTokens["easingAccelerate"] = QString("cubic-bezier(0.9, 0, 0.1, 1)"); // 加速
    _designTokens["easingDecelerate"] = QString("cubic-bezier(0.1, 0, 0.9, 1)"); // 减速
    _designTokens["easingLinear"]     = QString("linear");                       // 线性
}

// 初始化亮色主题颜色
void NTheme::NThemePrivate::initLightColors() {
    // 文本颜色 - Windows 11 亮色模式规范
    _lightColors[NTheme::TextPrimary]   = QColor("#000000"); // 主要文本
    _lightColors[NTheme::TextSecondary] = QColor("#5D5D5D"); // 次要文本
    _lightColors[NTheme::TextTertiary]  = QColor("#919191"); // 第三级文本
    _lightColors[NTheme::TextDisabled]  = QColor("#BDBDBD"); // 禁用文本
    _lightColors[NTheme::TextOnAccent]  = QColor("#FFFFFF"); // 强调色上的文本
    _lightColors[NTheme::TextLink]      = QColor("#0078D4"); // 链接文本

    // 背景颜色 - Windows 11 亮色模式
    _lightColors[NTheme::Background]          = QColor("#F9F9F9"); // 主背景
    _lightColors[NTheme::BackgroundSecondary] = QColor("#F3F3F3"); // 次级背景
    _lightColors[NTheme::BackgroundTertiary]  = QColor("#EEEEEE"); // 第三级背景
    _lightColors[NTheme::Surface]             = QColor("#FFFFFF"); // 表面
    _lightColors[NTheme::SurfaceSecondary]    = QColor("#F9F9F9"); // 次级表面

    // 控件颜色 - Windows 11 亮色模式规范
    _lightColors[NTheme::ControlFill]         = QColor("#F9F9F9");
    _lightColors[NTheme::ControlFillHover]    = QColor("#F3F3F3");
    _lightColors[NTheme::ControlFillPressed]  = QColor("#EEEEEE");
    _lightColors[NTheme::ControlFillSelected] = QColor("#E6F2FF");
    _lightColors[NTheme::ControlFillDisabled] = QColor("#F9F9F9");

    _lightColors[NTheme::ControlStroke]         = QColor("#CCCCCC");
    _lightColors[NTheme::ControlStrokeHover]    = QColor("#BDBDBD");
    _lightColors[NTheme::ControlStrokePressed]  = QColor("#ADADAD");
    _lightColors[NTheme::ControlStrokeSelected] = QColor("#0078D4");
    _lightColors[NTheme::ControlStrokeDisabled] = QColor("#E0E0E0");

    // 卡片颜色
    _lightColors[NTheme::CardBackground]        = QColor("#FFFFFF");
    _lightColors[NTheme::CardBackgroundHover]   = QColor("#F9F9F9");
    _lightColors[NTheme::CardBackgroundPressed] = QColor("#F3F3F3");
    _lightColors[NTheme::CardStroke]            = QColor("#E0E0E0");
    _lightColors[NTheme::CardStrokeHover]       = QColor("#CCCCCC");

    // 状态颜色 - Windows 11 亮色模式
    _lightColors[NTheme::Success] = QColor("#0F7B0F"); // 成功
    _lightColors[NTheme::Warning] = QColor("#9D5D00"); // 警告
    _lightColors[NTheme::Error]   = QColor("#C42B1C"); // 错误
    _lightColors[NTheme::Info]    = QColor("#0078D4"); // 信息

    // 分隔线颜色
    _lightColors[NTheme::Divider]       = QColor("#E0E0E0"); // 分隔线
    _lightColors[NTheme::DividerStrong] = QColor("#CCCCCC"); // 强调分隔线

    // 阴影颜色
    _lightColors[NTheme::Shadow] = QColor(0, 0, 0, 20); // 阴影

    // 添加 Fluent Design 颜色 - 亮色模式
    _lightColors["yellowGold"]   = QColor("#FFB900");
    _lightColors["gold"]         = QColor("#FF8C00");
    _lightColors["orangeBright"] = QColor("#F7630C");
    _lightColors["orangeDark"]   = QColor("#CA5010");
    _lightColors["rust"]         = QColor("#DA3B01");
    _lightColors["paleRust"]     = QColor("#EF6950");

    _lightColors["brickRed"]   = QColor("#D13438");
    _lightColors["modRed"]     = QColor("#FF4343");
    _lightColors["paleRed"]    = QColor("#E74856");
    _lightColors["red"]        = QColor("#E81123");
    _lightColors["roseBright"] = QColor("#EA005E");
    _lightColors["rose"]       = QColor("#C30052");

    _lightColors["plumLight"]   = QColor("#E3008C");
    _lightColors["plum"]        = QColor("#BF0077");
    _lightColors["orchidLight"] = QColor("#C239B3");
    _lightColors["orchid"]      = QColor("#9A0089");
    _lightColors["defaultBlue"] = QColor("#0078D7");
    _lightColors["navyBlue"]    = QColor("#0063B1");

    _lightColors["purpleShadow"]     = QColor("#8E8CD8");
    _lightColors["purpleShadowDark"] = QColor("#6B69D6");
    _lightColors["irisPastel"]       = QColor("#8764B8");
    _lightColors["irisSpring"]       = QColor("#744DA9");
    _lightColors["violetRedLight"]   = QColor("#B146C2");
    _lightColors["violetRed"]        = QColor("#881798");

    _lightColors["coolBlueBright"] = QColor("#0099BC");
    _lightColors["coolBlue"]       = QColor("#2D7D9A");
    _lightColors["seafoam"]        = QColor("#00B7C3");
    _lightColors["seafoamTeal"]    = QColor("#038387");
    _lightColors["mintLight"]      = QColor("#00B294");
    _lightColors["mintDark"]       = QColor("#018574");

    _lightColors["turfGreen"]  = QColor("#00CC6A");
    _lightColors["sportGreen"] = QColor("#10893E");
    _lightColors["gray"]       = QColor("#7A7574");
    _lightColors["grayBrown"]  = QColor("#5D5A58");
    _lightColors["steelBlue"]  = QColor("#68768A");
    _lightColors["metalBlue"]  = QColor("#515C6B");

    _lightColors["paleMoss"]    = QColor("#567C73");
    _lightColors["moss"]        = QColor("#486860");
    _lightColors["meadowGreen"] = QColor("#498205");
    _lightColors["green"]       = QColor("#107C10");
    _lightColors["overcast"]    = QColor("#767676");
    _lightColors["storm"]       = QColor("#4C4A48");

    _lightColors["blueGray"]         = QColor("#69797E");
    _lightColors["grayDark"]         = QColor("#4A5459");
    _lightColors["liddyGreen"]       = QColor("#647C64");
    _lightColors["sage"]             = QColor("#525E54");
    _lightColors["camouflageDesert"] = QColor("#847545");
    _lightColors["camouflage"]       = QColor("#7E735F");
}

// 初始化暗色主题颜色
void NTheme::NThemePrivate::initDarkColors() {
    // 文本颜色 - Windows 11 暗色模式规范
    _darkColors[NTheme::TextPrimary]   = QColor("#FFFFFF"); // 主要文本
    _darkColors[NTheme::TextSecondary] = QColor("#CCCCCC"); // 次要文本
    _darkColors[NTheme::TextTertiary]  = QColor("#9A9A9A"); // 第三级文本
    _darkColors[NTheme::TextDisabled]  = QColor("#5D5D5D"); // 禁用文本
    _darkColors[NTheme::TextOnAccent]  = QColor("#000000"); // 强调色上的文本
    _darkColors[NTheme::TextLink]      = QColor("#4CC2FF"); // 链接文本

    // 背景颜色 - Windows 11 暗色模式
    _darkColors[NTheme::Background]          = QColor("#202020"); // 主背景
    _darkColors[NTheme::BackgroundSecondary] = QColor("#272727"); // 次级背景
    _darkColors[NTheme::BackgroundTertiary]  = QColor("#2C2C2C"); // 第三级背景
    _darkColors[NTheme::Surface]             = QColor("#323232"); // 表面
    _darkColors[NTheme::SurfaceSecondary]    = QColor("#2E2E2E"); // 次级表面

    // 控件颜色 - Windows 11 暗色模式规范
    _darkColors[NTheme::ControlFill]         = QColor("#424242");
    _darkColors[NTheme::ControlFillHover]    = QColor("#5B5B5B");
    _darkColors[NTheme::ControlFillPressed]  = QColor("#6E6E6E");
    _darkColors[NTheme::ControlFillSelected] = QColor("#0A3C5A");
    _darkColors[NTheme::ControlFillDisabled] = QColor("#2C2C2C");

    _darkColors[NTheme::ControlStroke]         = QColor("#636363");
    _darkColors[NTheme::ControlStrokeHover]    = QColor("#818181");
    _darkColors[NTheme::ControlStrokePressed]  = QColor("#9C9C9C");
    _darkColors[NTheme::ControlStrokeSelected] = QColor("#60CDFF");
    _darkColors[NTheme::ControlStrokeDisabled] = QColor("#434343");

    // 卡片颜色
    _darkColors[NTheme::CardBackground]        = QColor("#323232");
    _darkColors[NTheme::CardBackgroundHover]   = QColor("#383838");
    _darkColors[NTheme::CardBackgroundPressed] = QColor("#404040");
    _darkColors[NTheme::CardStroke]            = QColor("#484848");
    _darkColors[NTheme::CardStrokeHover]       = QColor("#5A5A5A");

    // 状态颜色 - Windows 11 暗色模式
    _darkColors[NTheme::Success] = QColor("#6CCB5F"); // 成功
    _darkColors[NTheme::Warning] = QColor("#FCE100"); // 警告
    _darkColors[NTheme::Error]   = QColor("#FF99A4"); // 错误
    _darkColors[NTheme::Info]    = QColor("#60CDFF"); // 信息

    // 分隔线颜色
    _darkColors[NTheme::Divider]       = QColor("#474747"); // 分隔线
    _darkColors[NTheme::DividerStrong] = QColor("#636363"); // 强调分隔线

    // 阴影颜色
    _darkColors[NTheme::Shadow] = QColor(0, 0, 0, 70); // 阴影 (暗色下透明度更高)

    // 添加 Fluent Design 颜色 - 暗色模式下的调整版本
    _darkColors["yellowGold"]   = QColor("#FFB900").lighter(110);
    _darkColors["gold"]         = QColor("#FF8C00").lighter(110);
    _darkColors["orangeBright"] = QColor("#F7630C").lighter(110);
    _darkColors["orangeDark"]   = QColor("#CA5010").lighter(110);
    _darkColors["rust"]         = QColor("#DA3B01").lighter(110);
    _darkColors["paleRust"]     = QColor("#EF6950").lighter(110);

    _darkColors["brickRed"]   = QColor("#D13438").lighter(110);
    _darkColors["modRed"]     = QColor("#FF4343").lighter(110);
    _darkColors["paleRed"]    = QColor("#E74856").lighter(110);
    _darkColors["red"]        = QColor("#E81123").lighter(110);
    _darkColors["roseBright"] = QColor("#EA005E").lighter(110);
    _darkColors["rose"]       = QColor("#C30052").lighter(110);

    _darkColors["plumLight"]   = QColor("#E3008C").lighter(110);
    _darkColors["plum"]        = QColor("#BF0077").lighter(110);
    _darkColors["orchidLight"] = QColor("#C239B3").lighter(110);
    _darkColors["orchid"]      = QColor("#9A0089").lighter(110);
    _darkColors["defaultBlue"] = QColor("#0078D7").lighter(110);
    _darkColors["navyBlue"]    = QColor("#0063B1").lighter(110);

    _darkColors["purpleShadow"]     = QColor("#8E8CD8").lighter(110);
    _darkColors["purpleShadowDark"] = QColor("#6B69D6").lighter(110);
    _darkColors["irisPastel"]       = QColor("#8764B8").lighter(110);
    _darkColors["irisSpring"]       = QColor("#744DA9").lighter(110);
    _darkColors["violetRedLight"]   = QColor("#B146C2").lighter(110);
    _darkColors["violetRed"]        = QColor("#881798").lighter(110);

    _darkColors["coolBlueBright"] = QColor("#0099BC").lighter(110);
    _darkColors["coolBlue"]       = QColor("#2D7D9A").lighter(110);
    _darkColors["seafoam"]        = QColor("#00B7C3").lighter(110);
    _darkColors["seafoamTeal"]    = QColor("#038387").lighter(110);
    _darkColors["mintLight"]      = QColor("#00B294").lighter(110);
    _darkColors["mintDark"]       = QColor("#018574").lighter(110);

    _darkColors["turfGreen"]  = QColor("#00CC6A").lighter(110);
    _darkColors["sportGreen"] = QColor("#10893E").lighter(110);
    _darkColors["gray"]       = QColor("#7A7574").lighter(110);
    _darkColors["grayBrown"]  = QColor("#5D5A58").lighter(110);
    _darkColors["steelBlue"]  = QColor("#68768A").lighter(110);
    _darkColors["metalBlue"]  = QColor("#515C6B").lighter(110);

    _darkColors["paleMoss"]    = QColor("#567C73").lighter(110);
    _darkColors["moss"]        = QColor("#486860").lighter(110);
    _darkColors["meadowGreen"] = QColor("#498205").lighter(110);
    _darkColors["green"]       = QColor("#107C10").lighter(110);
    _darkColors["overcast"]    = QColor("#767676").lighter(110);
    _darkColors["storm"]       = QColor("#4C4A48").lighter(110);

    _darkColors["blueGray"]         = QColor("#69797E").lighter(110);
    _darkColors["grayDark"]         = QColor("#4A5459").lighter(110);
    _darkColors["liddyGreen"]       = QColor("#647C64").lighter(110);
    _darkColors["sage"]             = QColor("#525E54").lighter(110);
    _darkColors["camouflageDesert"] = QColor("#847545").lighter(110);
    _darkColors["camouflage"]       = QColor("#7E735F").lighter(110);
}

// 解析颜色 - 考虑当前主题模式和自定义颜色
QColor NTheme::NThemePrivate::resolveColor(const QString& key) const {
    // 首先检查自定义颜色
    if (_customColors.contains(key)) {
        return _customColors[key];
    }

    // 然后根据当前主题模式选择颜色
    if (_isDark) {
        if (_darkColors.contains(key)) {
            return _darkColors[key];
        }
    } else {
        if (_lightColors.contains(key)) {
            return _lightColors[key];
        }
    }

    // 如果找不到颜色，返回默认颜色
    return _isDark ? QColor("#FFFFFF") : QColor("#000000");
}

// 解析设计令牌
QVariant NTheme::NThemePrivate::resolveToken(const QString& key) const {
    // 首先检查自定义令牌
    if (_customTokens.contains(key)) {
        return _customTokens[key];
    }

    // 然后检查默认令牌
    if (_designTokens.contains(key)) {
        return _designTokens[key];
    }

    // 如果找不到令牌，返回空值
    return QVariant();
}

// 设置系统主题检测
void NTheme::NThemePrivate::setupSystemThemeDetection() {
    // 连接应用程序调色板变化信号
    QObject::connect(qApp, &QApplication::paletteChanged, [this]() {
        if (_themeMode == NTheme::ThemeMode::System) {
            updateThemeBasedOnSystem();
        }
    });

    // 初始检测
    updateThemeBasedOnSystem();
}

// 根据系统主题更新
void NTheme::NThemePrivate::updateThemeBasedOnSystem() {
    if (_themeMode == NTheme::ThemeMode::System) {
        // 检测系统主题
        QPalette palette    = qApp->palette();
        QColor   windowText = palette.color(QPalette::WindowText);
        QColor   window     = palette.color(QPalette::Window);

        // 如果窗口文本比窗口背景亮，则认为是暗色主题
        bool isDark = windowText.lightness() > window.lightness();

        if (_isDark != isDark) {
            _isDark = isDark;
            Q_Q(NTheme);
            emit q->darkModeChanged(_isDark);

            // 发出所有颜色变化的信号
            for (auto it = _lightColors.constBegin(); it != _lightColors.constEnd(); ++it) {
                emit q->colorChanged(it.key(), q->getColor(it.key()));
            }
        }
    }
}

// 更新暗色模式状态
void NTheme::NThemePrivate::updateDarkModeState() {
    bool newIsDark = (_themeMode == NTheme::ThemeMode::Dark);

    if (_isDark != newIsDark) {
        _isDark = newIsDark;
        Q_Q(NTheme);
        emit q->darkModeChanged(_isDark);

        // 发出所有颜色变化的信号
        for (auto it = _lightColors.constBegin(); it != _lightColors.constEnd(); ++it) {
            emit q->colorChanged(it.key(), q->getColor(it.key()));
        }
    }
}