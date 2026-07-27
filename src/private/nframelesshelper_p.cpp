#include "nframelesshelper_p.h"

#include <QAbstractButton>
#include <QApplication>
#include <QOperatingSystemVersion>
#include <QPalette>
#include <QWidget>

#include "QtNativeUI/NTheme.h"
#include <QWKWidgets/widgetwindowagent.h>

NFramelessHelper::NFramelessHelper(QWidget* host, QObject* parent)
    : QObject(parent)
    , m_host(host)
{
}

NFramelessHelper::~NFramelessHelper() = default;

void NFramelessHelper::setup()
{
    if (!m_host || m_windowAgent)
        return;

    m_host->setAttribute(Qt::WA_DontCreateNativeAncestors);
    m_host->setAttribute(Qt::WA_TranslucentBackground);

    m_windowAgent = new QWK::WidgetWindowAgent(m_host);
    m_windowAgent->setup(m_host);

#ifdef Q_OS_MAC
    m_host->setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);
#endif

    setupThemeConnection();
    updateBackgroundColor();
}

void NFramelessHelper::setupThemeConnection()
{
#ifdef Q_OS_MAC
    connect(nTheme, &NTheme::themeModeChanged, this, [this]() {
        if (m_windowEffect == Blur) {
            m_windowAgent->setWindowAttribute(QStringLiteral("blur-effect"),
                                              nTheme->isDarkMode() ? "dark" : "light");
        }
        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(m_host, &event);
    });
#elif defined(Q_OS_WIN)
    m_windowAgent->setWindowAttribute(QStringLiteral("dark-mode"), nTheme->isDarkMode());

    connect(nTheme, &NTheme::themeModeChanged, this, [this]() {
        m_windowAgent->setWindowAttribute(QStringLiteral("dark-mode"), nTheme->isDarkMode());

        if (m_windowEffect == None) {
            updateBackgroundColor();
            QPalette pal = m_host->palette();
            pal.setColor(QPalette::Window, m_backgroundColor);
            m_host->setPalette(pal);
            m_host->update();
        }

        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(m_host, &event);
    });
#endif
}

void NFramelessHelper::updateBackgroundColor()
{
    m_backgroundColor = nTheme->isDarkMode()
        ? nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Dark)
        : nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Light);
}

bool NFramelessHelper::isGlassEffectSupported()
{
#ifdef Q_OS_MAC
    const auto version = QOperatingSystemVersion::current();
    return version.type() == QOperatingSystemVersion::MacOS && version.majorVersion() >= 26;
#else
    return false;
#endif
}

bool NFramelessHelper::isGlassEffect(WindowEffectType type) const
{
    return type == GlassRegular || type == GlassClear;
}

void NFramelessHelper::clearMacBackdropEffects()
{
#ifdef Q_OS_MAC
    m_windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), QStringLiteral("none"));
    m_windowAgent->setWindowAttribute(QStringLiteral("glass-effect"), QStringLiteral("none"));
#endif
}

NFramelessHelper::WindowEffectType NFramelessHelper::applyBackdropEffect(WindowEffectType type)
{
    bool needsRepaint = false;
    WindowEffectType applied = type;

#ifdef Q_OS_WIN
    static const QStringList effectKeys = {
        QString(),
        QStringLiteral("dwm-blur"),
        QStringLiteral("acrylic-material"),
        QStringLiteral("mica"),
        QStringLiteral("mica-alt")
    };

    for (int i = 1; i < effectKeys.size(); ++i) {
        m_windowAgent->setWindowAttribute(effectKeys[i], false);
    }

    if (type != None && type < effectKeys.size()) {
        m_windowAgent->setWindowAttribute(effectKeys[type], true);
        m_host->setPalette(QPalette());
        if (m_windowEffect == None) {
            needsRepaint = true;
        }
    } else {
        updateBackgroundColor();
        QPalette pal = m_host->palette();
        pal.setColor(QPalette::Window, m_backgroundColor);
        m_host->setPalette(pal);
        needsRepaint = true;
    }

#elif defined(Q_OS_MAC)
    if (type == Blur) {
        m_windowAgent->setWindowAttribute(QStringLiteral("glass-effect"), QStringLiteral("none"));
        m_windowAgent->setWindowAttribute(QStringLiteral("blur-effect"),
                                          nTheme->isDarkMode() ? "dark" : "light");
        m_host->setPalette(QPalette());
        if (m_windowEffect == None) {
            needsRepaint = true;
        }
    } else if (isGlassEffect(type)) {
        m_windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), QStringLiteral("none"));
        m_windowAgent->setWindowAttribute(QStringLiteral("glass-corner-radius"), m_glassCornerRadius);
        m_windowAgent->setWindowAttribute(QStringLiteral("glass-tint-color"),
                                          m_glassTintColor.isValid()
                                              ? QVariant::fromValue(m_glassTintColor)
                                              : QVariant(QStringLiteral("none")));
        const QString effect = type == GlassRegular ? QStringLiteral("regular")
                                                    : QStringLiteral("clear");
        if (!m_windowAgent->setWindowAttribute(QStringLiteral("glass-effect"), effect)) {
            m_windowAgent->setWindowAttribute(QStringLiteral("glass-effect"), QStringLiteral("none"));
            m_windowAgent->setWindowAttribute(QStringLiteral("blur-effect"),
                                              nTheme->isDarkMode() ? "dark" : "light");
            applied = Blur;
        }
        m_host->setPalette(QPalette());
        if (m_windowEffect == None) {
            needsRepaint = true;
        }
    } else {
        clearMacBackdropEffects();
        updateBackgroundColor();
        QPalette pal = m_host->palette();
        pal.setColor(QPalette::Window, m_backgroundColor);
        m_host->setPalette(pal);
        needsRepaint = true;
    }
#else
    Q_UNUSED(type)
#endif

    if (needsRepaint) {
        m_host->update();
    }

    return applied;
}

void NFramelessHelper::setWindowEffect(WindowEffectType type)
{
    if (m_windowEffect == type)
        return;

    const WindowEffectType applied = applyBackdropEffect(type);
    m_windowEffect = applied;
    emit windowEffectChanged(applied);
}

NFramelessHelper::WindowEffectType NFramelessHelper::windowEffect() const
{
    return m_windowEffect;
}

void NFramelessHelper::setGlassCornerRadius(qreal radius)
{
    if (qFuzzyCompare(m_glassCornerRadius, radius))
        return;

    m_glassCornerRadius = radius;
    if (isGlassEffect(m_windowEffect)) {
        m_windowAgent->setWindowAttribute(QStringLiteral("glass-corner-radius"), radius);
    }
}

qreal NFramelessHelper::glassCornerRadius() const
{
    return m_glassCornerRadius;
}

void NFramelessHelper::setGlassTintColor(const QColor& color)
{
    if (m_glassTintColor == color)
        return;

    m_glassTintColor = color;
    if (isGlassEffect(m_windowEffect)) {
        m_windowAgent->setWindowAttribute(QStringLiteral("glass-tint-color"),
                                          color.isValid() ? QVariant::fromValue(color)
                                                          : QVariant(QStringLiteral("none")));
    }
}

QColor NFramelessHelper::glassTintColor() const
{
    return m_glassTintColor;
}

int NFramelessHelper::borderThickness() const
{
    QVariant val = m_windowAgent->windowAttribute(QStringLiteral("border-thickness"));
    return val.isValid() ? val.toInt() : 0;
}

int NFramelessHelper::titleBarHeight() const
{
    QVariant val = m_windowAgent->windowAttribute(QStringLiteral("title-bar-height"));
    return val.isValid() ? val.toInt() : 32;
}

void NFramelessHelper::setTitleBar(QWidget* titleBar)
{
    if (m_windowAgent) {
        m_windowAgent->setTitleBar(titleBar);
    }
}

void NFramelessHelper::setHitTestVisible(QWidget* widget, bool visible)
{
    if (m_windowAgent) {
        m_windowAgent->setHitTestVisible(widget, visible);
    }
}

void NFramelessHelper::setSystemButton(SystemButtonType type, QAbstractButton* button)
{
    if (!m_windowAgent)
        return;

    QWK::WindowAgentBase::SystemButton qwkType;
    switch (type) {
        case WindowIcon:
            qwkType = QWK::WindowAgentBase::WindowIcon;
            break;
        case Minimize:
            qwkType = QWK::WindowAgentBase::Minimize;
            break;
        case Maximize:
            qwkType = QWK::WindowAgentBase::Maximize;
            break;
        case Close:
            qwkType = QWK::WindowAgentBase::Close;
            break;
        default:
            return;
    }

    m_windowAgent->setSystemButton(qwkType, button);
}

bool NFramelessHelper::setWindowAttribute(const QString& key, const QVariant& value)
{
    if (m_windowAgent) {
        return m_windowAgent->setWindowAttribute(key, value);
    }
    return false;
}

QVariant NFramelessHelper::windowAttribute(const QString& key) const
{
    if (m_windowAgent) {
        return m_windowAgent->windowAttribute(key);
    }
    return QVariant();
}

QWK::WidgetWindowAgent* NFramelessHelper::windowAgent() const
{
    return m_windowAgent;
}

QWidget* NFramelessHelper::hostWidget() const
{
    return m_host;
}

QColor NFramelessHelper::backgroundColor() const
{
    return m_backgroundColor;
}

#ifdef Q_OS_MAC
void NFramelessHelper::setSystemButtonAreaCallback(const std::function<QRect(const QSize&)>& callback)
{
    if (m_windowAgent) {
        m_windowAgent->setSystemButtonAreaCallback(callback);
    }
}

void NFramelessHelper::setNativeSystemButtonsVisible(bool visible)
{
    if (!m_windowAgent)
        return;

    if (!visible) {
        setWindowAttribute(QStringLiteral("no-system-buttons"), true);
        setSystemButtonAreaCallback([](const QSize& size) {
            constexpr int width = -75;
            return QRect(QPoint(size.width() - width, 0), QSize(width, size.height()));
        });
    } else {
        setWindowAttribute(QStringLiteral("no-system-buttons"), false);
        m_windowAgent->setSystemButtonAreaCallback(nullptr);
    }
}
#endif
