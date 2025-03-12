#include <QList>
#include <QtCore/qset.h>
#include <QtMath>
#include <QtNativeUI/NColor.h>

// NShadedColor 实现
NShadedColor::NShadedColor(const QColor& primary, const QMap<int, QColor>& swatch)
    : _primary(primary), _swatch(swatch) {}

QColor NShadedColor::operator[](int shade) const { return _swatch.contains(shade) ? _swatch[shade] : _primary; }

// NAccentColor 实现
NAccentColor::NAccentColor(const QMap<QString, QColor>& swatch) : _swatch(swatch) {}

NAccentColor NAccentColor::fromColor(const QColor& color,
                                     double        darkestFactor,
                                     double        darkerFactor,
                                     double        darkFactor,
                                     double        lightFactor,
                                     double        lighterFactor,
                                     double        lightestFactor) {
    QMap<QString, QColor> swatch;

    // 创建暗色调
    swatch["darkest"] = NColorUtils::lerpColor(color, NColors::black, darkestFactor);
    swatch["darker"]  = NColorUtils::lerpColor(color, NColors::black, darkerFactor);
    swatch["dark"]    = NColorUtils::lerpColor(color, NColors::black, darkFactor);

    // 基础色调
    swatch["normal"] = color;

    // 创建亮色调
    swatch["light"]    = NColorUtils::lerpColor(color, NColors::white, lightFactor);
    swatch["lighter"]  = NColorUtils::lerpColor(color, NColors::white, lighterFactor);
    swatch["lightest"] = NColorUtils::lerpColor(color, NColors::white, lightestFactor);

    return NAccentColor(swatch);
}

QColor NAccentColor::darkest() const {
    return _swatch.contains("darkest") ? _swatch["darkest"] : NColorUtils::withOpacity(darker(), 0.7);
}

QColor NAccentColor::darker() const {
    return _swatch.contains("darker") ? _swatch["darker"] : NColorUtils::withOpacity(dark(), 0.8);
}

QColor NAccentColor::dark() const {
    return _swatch.contains("dark") ? _swatch["dark"] : NColorUtils::withOpacity(normal(), 0.9);
}

QColor NAccentColor::normal() const { return _swatch["normal"]; }

QColor NAccentColor::light() const {
    return _swatch.contains("light") ? _swatch["light"] : NColorUtils::withOpacity(normal(), 0.9);
}

QColor NAccentColor::lighter() const {
    return _swatch.contains("lighter") ? _swatch["lighter"] : NColorUtils::withOpacity(light(), 0.8);
}

QColor NAccentColor::lightest() const {
    return _swatch.contains("lightest") ? _swatch["lightest"] : NColorUtils::withOpacity(lighter(), 0.7);
}

QColor NAccentColor::operator[](const QString& name) const {
    if (_swatch.contains(name)) {
        return _swatch[name];
    }

    // 默认返回 normal
    return normal();
}

QColor NAccentColor::defaultBrushFor(bool isDark) const { return isDark ? lighter() : dark(); }

QColor NAccentColor::secondaryBrushFor(bool isDark) const {
    QColor base = defaultBrushFor(isDark);
    return NColorUtils::withOpacity(base, 0.9);
}

QColor NAccentColor::tertiaryBrushFor(bool isDark) const {
    QColor base = defaultBrushFor(isDark);
    return NColorUtils::withOpacity(base, 0.8);
}

NAccentColor NAccentColor::lerp(const NAccentColor& a, const NAccentColor& b, double t) {
    QMap<QString, QColor> result;

    // 合并两个色调映射的键
    QSet<QString> keys;
    for (auto it = a._swatch.begin(); it != a._swatch.end(); ++it) {
        keys.insert(it.key());
    }
    for (auto it = b._swatch.begin(); it != b._swatch.end(); ++it) {
        keys.insert(it.key());
    }

    // 对每个键进行插值
    for (const QString& key : keys) {
        if (a._swatch.contains(key) && b._swatch.contains(key)) {
            result[key] = NColorUtils::lerpColor(a._swatch[key], b._swatch[key], t);
        } else if (a._swatch.contains(key)) {
            result[key] = a._swatch[key];
        } else if (b._swatch.contains(key)) {
            result[key] = b._swatch[key];
        }
    }

    // 确保至少有 normal 键
    if (!result.contains("normal")) {
        result["normal"] = NColorUtils::lerpColor(a.normal(), b.normal(), t);
    }

    return NAccentColor(result);
}

bool NAccentColor::operator==(const NAccentColor& other) const {
    // 比较主要色调
    if (normal() != other.normal()) {
        return false;
    }

    // 比较所有色调
    QSet<QString> allKeys;
    for (auto it = _swatch.begin(); it != _swatch.end(); ++it) {
        allKeys.insert(it.key());
    }
    for (auto it = other._swatch.begin(); it != other._swatch.end(); ++it) {
        allKeys.insert(it.key());
    }

    for (const QString& key : allKeys) {
        if (_swatch.contains(key) && other._swatch.contains(key)) {
            if (_swatch[key] != other._swatch[key]) {
                return false;
            }
        } else if (_swatch.contains(key) || other._swatch.contains(key)) {
            return false;
        }
    }

    return true;
}

bool NAccentColor::operator!=(const NAccentColor& other) const { return !(*this == other); }

// NColors 实现
const QColor NColors::transparent = QColor(0, 0, 0, 0);
const QColor NColors::black       = QColor(0, 0, 0);
const QColor NColors::white       = QColor(255, 255, 255);

// 灰色色调
const NShadedColor NColors::grey =
    NShadedColor(QColor(0x32, 0x31, 0x30), // grey160 作为主色调
                 QMap<int, QColor>(
                     {{220, QColor(0x11, 0x10, 0x0F)}, {210, QColor(0x16, 0x15, 0x14)}, {200, QColor(0x1B, 0x1A, 0x19)},
                      {190, QColor(0x20, 0x1F, 0x1E)}, {180, QColor(0x25, 0x24, 0x23)}, {170, QColor(0x29, 0x28, 0x27)},
                      {160, QColor(0x32, 0x31, 0x30)}, {150, QColor(0x3B, 0x3A, 0x39)}, {140, QColor(0x48, 0x46, 0x44)},
                      {130, QColor(0x60, 0x5E, 0x5C)}, {120, QColor(0x79, 0x77, 0x75)}, {110, QColor(0x8A, 0x88, 0x86)},
                      {100, QColor(0x97, 0x95, 0x93)}, {90, QColor(0xA1, 0x9F, 0x9D)},  {80, QColor(0xB3, 0xB0, 0xAD)},
                      {70, QColor(0xBE, 0xBB, 0xB8)},  {60, QColor(0xC8, 0xC6, 0xC4)},  {50, QColor(0xD2, 0xD0, 0xCE)},
                      {40, QColor(0xE1, 0xDF, 0xDD)},  {30, QColor(0xED, 0xEB, 0xE9)},  {20, QColor(0xF3, 0xF2, 0xF1)},
                      {10, QColor(0xFA, 0xF9, 0xF8)}}));

// 强调色
const NAccentColor NColors::yellow = NAccentColor(QMap<QString, QColor>({{"darkest", QColor(0xf9, 0xa8, 0x25)},
                                                                         {"darker", QColor(0xfb, 0xc0, 0x2d)},
                                                                         {"dark", QColor(0xfd, 0xd8, 0x35)},
                                                                         {"normal", QColor(0xff, 0xeb, 0x3b)},
                                                                         {"light", QColor(0xff, 0xee, 0x58)},
                                                                         {"lighter", QColor(0xff, 0xf1, 0x76)},
                                                                         {"lightest", QColor(0xff, 0xf5, 0x9d)}}));

const NAccentColor NColors::orange = NAccentColor(QMap<QString, QColor>({{"darkest", QColor(0x99, 0x3d, 0x07)},
                                                                         {"darker", QColor(0xac, 0x45, 0x08)},
                                                                         {"dark", QColor(0xd1, 0x54, 0x0a)},
                                                                         {"normal", QColor(0xf7, 0x63, 0x0c)},
                                                                         {"light", QColor(0xf8, 0x7a, 0x30)},
                                                                         {"lighter", QColor(0xf9, 0x91, 0x54)},
                                                                         {"lightest", QColor(0xfa, 0x9e, 0x68)}}));

const NAccentColor NColors::red = NAccentColor(QMap<QString, QColor>({{"darkest", QColor(0x8f, 0x0a, 0x15)},
                                                                      {"darker", QColor(0xa2, 0x0b, 0x18)},
                                                                      {"dark", QColor(0xb9, 0x0d, 0x1c)},
                                                                      {"normal", QColor(0xe8, 0x11, 0x23)},
                                                                      {"light", QColor(0xec, 0x40, 0x4f)},
                                                                      {"lighter", QColor(0xee, 0x58, 0x65)},
                                                                      {"lightest", QColor(0xf0, 0x6b, 0x76)}}));

const NAccentColor NColors::magenta = NAccentColor(QMap<QString, QColor>({{"darkest", QColor(0x6f, 0x00, 0x61)},
                                                                          {"darker", QColor(0x7e, 0x00, 0x6e)},
                                                                          {"dark", QColor(0x90, 0x00, 0x7e)},
                                                                          {"normal", QColor(0xb4, 0x00, 0x9e)},
                                                                          {"light", QColor(0xc3, 0x33, 0xb1)},
                                                                          {"lighter", QColor(0xca, 0x4c, 0xbb)},
                                                                          {"lightest", QColor(0xd0, 0x60, 0xc2)}}));

const NAccentColor NColors::purple = NAccentColor(QMap<QString, QColor>({{"darkest", QColor(0x47, 0x2f, 0x68)},
                                                                         {"darker", QColor(0x51, 0x35, 0x76)},
                                                                         {"dark", QColor(0x64, 0x42, 0x93)},
                                                                         {"normal", QColor(0x74, 0x4d, 0xa9)},
                                                                         {"light", QColor(0x86, 0x64, 0xb4)},
                                                                         {"lighter", QColor(0x9d, 0x82, 0xc2)},
                                                                         {"lightest", QColor(0xa8, 0x90, 0xc9)}}));

const NAccentColor NColors::blue = NAccentColor(QMap<QString, QColor>({{"darkest", QColor(0x00, 0x4a, 0x83)},
                                                                       {"darker", QColor(0x00, 0x54, 0x94)},
                                                                       {"dark", QColor(0x00, 0x66, 0xb4)},
                                                                       {"normal", QColor(0x00, 0x78, 0xd4)},
                                                                       {"light", QColor(0x26, 0x8c, 0xda)},
                                                                       {"lighter", QColor(0x4c, 0xa0, 0xe0)},
                                                                       {"lightest", QColor(0x60, 0xab, 0xe4)}}));

const NAccentColor NColors::teal = NAccentColor(QMap<QString, QColor>({{"darkest", QColor(0x00, 0x6e, 0x5b)},
                                                                       {"darker", QColor(0x00, 0x7c, 0x67)},
                                                                       {"dark", QColor(0x00, 0x97, 0x7d)},
                                                                       {"normal", QColor(0x00, 0xb2, 0x94)},
                                                                       {"light", QColor(0x26, 0xbd, 0xa4)},
                                                                       {"lighter", QColor(0x4c, 0xc9, 0xb4)},
                                                                       {"lightest", QColor(0x60, 0xcf, 0xbc)}}));

const NAccentColor NColors::green = NAccentColor(QMap<QString, QColor>({{"darkest", QColor(0x09, 0x4c, 0x09)},
                                                                        {"darker", QColor(0x0c, 0x5d, 0x0c)},
                                                                        {"dark", QColor(0x0e, 0x6f, 0x0e)},
                                                                        {"normal", QColor(0x10, 0x7c, 0x10)},
                                                                        {"light", QColor(0x27, 0x89, 0x27)},
                                                                        {"lighter", QColor(0x4b, 0x9c, 0x4b)},
                                                                        {"lightest", QColor(0x6a, 0xad, 0x6a)}}));

// 状态颜色
const QColor       NColors::warningPrimaryColor = QColor(0xd8, 0x3b, 0x01);
const NAccentColor NColors::warningSecondaryColor =
    NAccentColor(QMap<QString, QColor>({{"dark", QColor(0x43, 0x35, 0x19)}, {"normal", QColor(0xff, 0xf4, 0xce)}}));

const QColor       NColors::errorPrimaryColor = QColor(0xa8, 0x00, 0x00);
const NAccentColor NColors::errorSecondaryColor =
    NAccentColor(QMap<QString, QColor>({{"dark", QColor(0x44, 0x27, 0x26)}, {"normal", QColor(0xfd, 0xe7, 0xe9)}}));

const QColor       NColors::successPrimaryColor = QColor(0x10, 0x7c, 0x10);
const NAccentColor NColors::successSecondaryColor =
    NAccentColor(QMap<QString, QColor>({{"dark", QColor(0x39, 0x3d, 0x1b)}, {"normal", QColor(0xdf, 0xf6, 0xdd)}}));

NAccentColor NColors::getAccentColor(NAccentColorType::Type type) {
    switch (type) {
        case NAccentColorType::Yellow:
            return NColors::yellow;
        case NAccentColorType::Orange:
            return NColors::orange;
        case NAccentColorType::Red:
            return NColors::red;
        case NAccentColorType::Magenta:
            return NColors::magenta;
        case NAccentColorType::Purple:
            return NColors::purple;
        case NAccentColorType::Blue:
            return NColors::blue;
        case NAccentColorType::Teal:
            return NColors::teal;
        case NAccentColorType::Green:
            return NColors::green;
        case NAccentColorType::Custom:
        default:
            // 默认返回蓝色
            return NColors::blue;
    }
}

// 所有强调色列表
const QList<NAccentColor> NColors::accentColors = {NColors::yellow,
                                                   NColors::orange,
                                                   NColors::red,
                                                   NColors::magenta,
                                                   NColors::purple,
                                                   NColors::blue,
                                                   NColors::teal,
                                                   NColors::green};

// NColorUtils 实现
QColor NColorUtils::contrastColor(const QColor& color, const QColor& darkColor, const QColor& lightColor) {
    // 计算颜色的亮度 (使用相对亮度公式)
    double luminance = 0.299 * color.redF() + 0.587 * color.greenF() + 0.114 * color.blueF();
    return luminance < 0.5 ? lightColor : darkColor;
}

QColor NColorUtils::lerpColor(const QColor& a, const QColor& b, double t) {
    return QColor(qRound(a.red() + (b.red() - a.red()) * t),
                  qRound(a.green() + (b.green() - a.green()) * t),
                  qRound(a.blue() + (b.blue() - a.blue()) * t),
                  qRound(a.alpha() + (b.alpha() - a.alpha()) * t));
}

NAccentColor NColorUtils::toAccentColor(const QColor& color) { return NAccentColor::fromColor(color); }

QColor NColorUtils::adjustBrightness(const QColor& color, double factor) {
    int r = qBound(0, qRound(color.red() * factor), 255);
    int g = qBound(0, qRound(color.green() * factor), 255);
    int b = qBound(0, qRound(color.blue() * factor), 255);
    return QColor(r, g, b, color.alpha());
}

QColor NColorUtils::withAlpha(const QColor& color, int alpha) {
    QColor result = color;
    result.setAlpha(qBound(0, alpha, 255));
    return result;
}

QColor NColorUtils::withOpacity(const QColor& color, double opacity) {
    return withAlpha(color, qRound(color.alpha() * opacity));
}