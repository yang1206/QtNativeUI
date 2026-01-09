#include "QtNativeUI/NIcon.h"
#include <QFontDatabase>
#include <QPainter>
#include <QtGui/qguiapplication.h>

#include "QtNativeUI/NTheme.h"

NFluentIconEngine::NFluentIconEngine(bool isRegular, uint32_t iconCode, int size, const QColor& customColor)
    : isRegular(isRegular), iconCode(iconCode), iconSize(size), customColor(customColor) {}

NFluentIconEngine::~NFluentIconEngine() = default;

QColor NFluentIconEngine::getEffectiveColor(QIcon::Mode mode) const {
    if (customColor.isValid()) {
        if (mode == QIcon::Disabled) {
            QColor disabledColor = customColor;
            disabledColor.setAlpha(128);
            return disabledColor;
        }
        return customColor;
    }

    QColor themeColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, nTheme->themeMode());

    if (mode == QIcon::Disabled) {
        themeColor.setAlpha(128);
    }

    return themeColor;
}

void NFluentIconEngine::paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) {
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    QColor iconColor = getEffectiveColor(mode);
    painter->setPen(iconColor);

    QString fontFamily;
    if (isRegular) {
        fontFamily = nIcon->getRegularFontFamily();
    } else {
        fontFamily = nIcon->getFilledFontFamily();
    }

    QFont iconFont(fontFamily);

    int effectiveSize = qMin(rect.width(), rect.height());
    if (effectiveSize > 0) {
        iconFont.setPixelSize(effectiveSize * 0.9);
    } else {
        iconFont.setPixelSize(iconSize);
    }

    painter->setFont(iconFont);

    QString iconText = nIcon->getIconText(iconCode);
    painter->drawText(rect, Qt::AlignCenter, iconText);

    painter->restore();
}

QIconEngine* NFluentIconEngine::clone() const {
    return new NFluentIconEngine(isRegular, iconCode, iconSize, customColor);
}

QSize NFluentIconEngine::actualSize(const QSize& size, QIcon::Mode mode, QIcon::State state) {
    if (size.isValid() && !size.isNull()) {
        return size;
    }
    return QSize(iconSize, iconSize);
}

QPixmap NFluentIconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) {
    QSize effectiveSize = actualSize(size, mode, state);

    QPixmap pix(effectiveSize);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    paint(&painter, QRect(QPoint(0, 0), effectiveSize), mode, state);

    return pix;
}

Q_SINGLETON_CREATE_CPP(NIcon)

NIcon::NIcon() { initFontConfigs(); }

NIcon::~NIcon() = default;

void NIcon::initFontConfigs() {
    fontConfigs["Regular"] = FontConfig("FluentSystemIcons-Regular", 24);
    fontConfigs["Filled"]  = FontConfig("FluentSystemIcons-Filled", 24);

    QStringList fontFiles = {":/fonts/fonts/FluentSystemIcons-Regular.ttf",
                             ":/fonts/fonts/FluentSystemIcons-Filled.ttf"};

    for (const QString& fontFile : fontFiles) {
        int fontId = QFontDatabase::addApplicationFont(fontFile);
        if (fontId < 0) {
            qWarning() << "Failed to load font file:" << fontFile;
        } else {
            QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
            if (fontFile.contains("Regular")) {
                fontConfigs["Regular"].fontFamily = family;
            } else {
                fontConfigs["Filled"].fontFamily = family;
            }
        }
    }
}

QColor NIcon::getThemeBasedColor() const {
    bool isDark = nTheme->isDarkMode();
    if (isDark) {
        return NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::ThemeMode::Dark);
    } else {
        return NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::ThemeMode::Light);
    }
}

QString NIcon::getIconText(uint32_t unicode) {
    QString iconText;
    if (unicode <= 0xFFFF) {
        iconText = QChar(unicode);
    } else {
        char32_t ucs4Char = unicode;
        iconText          = QString::fromUcs4(&ucs4Char, 1).toUpper();
    }
    return iconText;
}

QIcon NIcon::createThemeAwareIcon(bool isRegular, uint32_t unicode, int size, const QColor& customColor) {
    return QIcon(new NFluentIconEngine(isRegular, unicode, size, customColor));
}

QIcon NIcon::createIcon(const QString& fontFamily,
                        uint32_t       unicode,
                        int            size,
                        const QColor&  color,
                        int            width,
                        int            height) {
    bool isRegular = fontFamily == fontConfigs["Regular"].fontFamily;

    if (!color.isValid() && width == 0 && height == 0) {
        return createThemeAwareIcon(isRegular, unicode, size);
    }

    QFont iconFont(fontFamily);

    qreal dpr          = qApp->devicePixelRatio();
    int   actualWidth  = (width > 0 ? width : size) * dpr;
    int   actualHeight = (height > 0 ? height : size) * dpr;

    QPixmap pix(actualWidth, actualHeight);
    pix.setDevicePixelRatio(dpr);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    QColor iconColor = color.isValid() ? color : getThemeBasedColor();
    painter.setPen(iconColor);
    iconFont.setPixelSize(size);
    painter.setFont(iconFont);

    QString iconText = getIconText(unicode);
    painter.drawText(0, 0, actualWidth / dpr, actualHeight / dpr, Qt::AlignCenter, iconText);

    return QIcon(pix);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon) {
    return createThemeAwareIcon(true, static_cast<unsigned int>(icon), fontConfigs["Regular"].defaultSize);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, const QColor& color) {
    return createThemeAwareIcon(true, static_cast<unsigned int>(icon), fontConfigs["Regular"].defaultSize, color);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, int size) {
    return createThemeAwareIcon(true, static_cast<unsigned int>(icon), size);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, int size, const QColor& color) {
    return createThemeAwareIcon(true, static_cast<unsigned int>(icon), size, color);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, int size, int width, int height) {
    return createIcon(
        fontConfigs["Regular"].fontFamily, static_cast<unsigned int>(icon), size, Qt::black, width, height);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, int size, int width, int height, const QColor& color) {
    return createIcon(fontConfigs["Regular"].fontFamily, static_cast<unsigned int>(icon), size, color, width, height);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon) {
    return createThemeAwareIcon(false, static_cast<unsigned int>(icon), fontConfigs["Filled"].defaultSize);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, const QColor& color) {
    return createThemeAwareIcon(false, static_cast<unsigned int>(icon), fontConfigs["Filled"].defaultSize, color);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, int size) {
    return createThemeAwareIcon(false, static_cast<unsigned int>(icon), size);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, int size, const QColor& color) {
    return createThemeAwareIcon(false, static_cast<unsigned int>(icon), size, color);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, int size, int width, int height) {
    return createIcon(
        fontConfigs["Filled"].fontFamily, static_cast<unsigned int>(icon), size, Qt::black, width, height);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, int size, int width, int height, const QColor& color) {
    return createIcon(fontConfigs["Filled"].fontFamily, static_cast<unsigned int>(icon), size, color, width, height);
}