#include "QtNativeUI/NIcon.h"
#include <QDebug>
#include <QFontDatabase>
#include <QPainter>
#include <QPixmap>
#include <QtGui/qguiapplication.h>

Q_SINGLETON_CREATE_CPP(NIcon)

NIcon::NIcon() { initFontConfigs(); }

NIcon::~NIcon() = default;

void NIcon::initFontConfigs() {
    fontConfigs["Regular"] = FontConfig("FluentSystemIcons-Regular", 24);
    fontConfigs["Filled"]  = FontConfig("FluentSystemIcons-Filled", 24);

    // 加载字体文件
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

QIcon NIcon::createIcon(const QString& fontFamily,
                        uint32_t       unicode,
                        int            size,
                        const QColor&  color,
                        int            width,
                        int            height) {
    QFont iconFont(fontFamily);

    qreal dpr          = qApp->devicePixelRatio();
    int   actualWidth  = (width > 0 ? width : size) * dpr;
    int   actualHeight = (height > 0 ? height : size) * dpr;

    QPixmap pix(actualWidth, actualHeight);
    pix.setDevicePixelRatio(dpr);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    painter.setPen(color);
    iconFont.setPixelSize(size);
    painter.setFont(iconFont);

    QString iconText;
    if (unicode <= 0xFFFF) {
        // 传统方式处理BMP平面字符
        iconText = QChar(unicode);
    } else {
        // 使用fromUcs4处理高位Unicode码点
        uint ucs4Char = unicode;
        iconText      = QString::fromUcs4(&ucs4Char, 1);
    }

    painter.drawText(0, 0, actualWidth / dpr, actualHeight / dpr, Qt::AlignCenter, iconText);

    return QIcon(pix);
}

// Regular 图标实现
QIcon NIcon::fromRegular(NRegularIconType::Icon icon) {
    return createIcon(fontConfigs["Regular"].fontFamily, static_cast<unsigned int>(icon));
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, const QColor& color) {
    return createIcon(
        fontConfigs["Regular"].fontFamily, static_cast<unsigned int>(icon), fontConfigs["Regular"].defaultSize, color);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, int size) {
    return createIcon(fontConfigs["Regular"].fontFamily, static_cast<unsigned int>(icon), size);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, int size, const QColor& color) {
    return createIcon(fontConfigs["Regular"].fontFamily, static_cast<unsigned int>(icon), size, color);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, int size, int width, int height) {
    return createIcon(
        fontConfigs["Regular"].fontFamily, static_cast<unsigned int>(icon), size, Qt::black, width, height);
}

QIcon NIcon::fromRegular(NRegularIconType::Icon icon, int size, int width, int height, const QColor& color) {
    return createIcon(fontConfigs["Regular"].fontFamily, static_cast<unsigned int>(icon), size, color, width, height);
}

// Filled 图标实现
QIcon NIcon::fromFilled(NFilledIconType::Icon icon) {
    return createIcon(fontConfigs["Filled"].fontFamily, static_cast<unsigned int>(icon));
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, const QColor& color) {
    return createIcon(
        fontConfigs["Filled"].fontFamily, static_cast<unsigned int>(icon), fontConfigs["Filled"].defaultSize, color);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, int size) {
    return createIcon(fontConfigs["Filled"].fontFamily, static_cast<unsigned int>(icon), size);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, int size, const QColor& color) {
    return createIcon(fontConfigs["Filled"].fontFamily, static_cast<unsigned int>(icon), size, color);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, int size, int width, int height) {
    return createIcon(
        fontConfigs["Filled"].fontFamily, static_cast<unsigned int>(icon), size, Qt::black, width, height);
}

QIcon NIcon::fromFilled(NFilledIconType::Icon icon, int size, int width, int height, const QColor& color) {
    return createIcon(fontConfigs["Filled"].fontFamily, static_cast<unsigned int>(icon), size, color, width, height);
}