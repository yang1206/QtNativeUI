/**
 * @brief Frameless window helper for QWindowKit integration
 *
 * This is a private implementation class that encapsulates QWindowKit operations
 * for frameless windows. It can be used by NMainWindow, NDialog, or any QWidget
 * that needs frameless window functionality.
 */

#ifndef QTNATIVEUI_NFRAMELESSHELPER_P_H
#define QTNATIVEUI_NFRAMELESSHELPER_P_H

#include <QColor>
#include <QObject>
#include <QVariant>

class QWidget;
class QAbstractButton;

namespace QWK {
class WidgetWindowAgent;
}

class NFramelessHelper : public QObject {
    Q_OBJECT

  public:
    enum WindowEffectType { None = 0, Blur, Acrylic, Mica, MicaAlt };

    enum SystemButtonType { WindowIcon, Minimize, Maximize, Close };

    explicit NFramelessHelper(QWidget* host, QObject* parent = nullptr);
    ~NFramelessHelper() override;

    void setup();

    void         setWindowEffect(WindowEffectType type);
    WindowEffectType windowEffect() const;

    int borderThickness() const;
    int titleBarHeight() const;

    void setTitleBar(QWidget* titleBar);
    void setHitTestVisible(QWidget* widget, bool visible);
    void setSystemButton(SystemButtonType type, QAbstractButton* button);

    bool     setWindowAttribute(const QString& key, const QVariant& value);
    QVariant windowAttribute(const QString& key) const;

    QWK::WidgetWindowAgent* windowAgent() const;
    QWidget*                hostWidget() const;
    QColor                  backgroundColor() const;

#ifdef Q_OS_MAC
    void setSystemButtonAreaCallback(const std::function<QRect(const QSize&)>& callback);
    void setNativeSystemButtonsVisible(bool visible);
#endif

    static constexpr bool useNativeSystemButtons() {
#ifdef Q_OS_MAC
        return true;
#else
        return false;
#endif
    }

    static constexpr bool supportsMica() {
#ifdef Q_OS_WIN
        return true;
#else
        return false;
#endif
    }

    static constexpr bool supportsAcrylic() {
#ifdef Q_OS_WIN
        return true;
#else
        return false;
#endif
    }

  Q_SIGNALS:
    void windowEffectChanged(WindowEffectType type);

  private:
    void setupThemeConnection();
    void applyBackdropEffect(WindowEffectType type);
    void updateBackgroundColor();

    QWidget*                m_host         = nullptr;
    QWK::WidgetWindowAgent* m_windowAgent  = nullptr;
    WindowEffectType        m_windowEffect = None;
    QColor                  m_backgroundColor;
};

#endif // QTNATIVEUI_NFRAMELESSHELPER_P_H
