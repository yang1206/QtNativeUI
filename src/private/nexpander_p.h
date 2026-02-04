#ifndef QTNATIVEUI_NEXPANDER_P_H
#define QTNATIVEUI_NEXPANDER_P_H

#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NExpander.h>
#include <QWidget>

class QPropertyAnimation;
class QVariantAnimation;
class QVBoxLayout;

class NExpanderHeaderWidget : public QWidget {
    Q_OBJECT

  public:
    explicit NExpanderHeaderWidget(QWidget* parent = nullptr);

    void    setHeader(const QString& header);
    QString header() const { return m_header; }

    void setHeaderIcon(bool isRegular, quint32 iconCode, int size);
    void clearHeaderIcon();

    void setChevronRotation(qreal rotation);
    void setExpanded(bool expanded);

  signals:
    void clicked();

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

  private:
    QString m_header;
    bool    m_isHovered{false};
    bool    m_isPressed{false};
    bool    m_expanded{false};
    qreal   m_chevronRotation{0.0};

    struct HeaderIconInfo {
        bool    isRegular = true;
        quint32 iconCode  = 0;
        int     size      = 16;
        bool    hasIcon   = false;
    } m_headerIcon;
};

class NExpanderContentWidget : public QWidget {
    Q_OBJECT

  public:
    explicit NExpanderContentWidget(QWidget* parent = nullptr);

    QVBoxLayout* layout() const { return m_layout; }

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    QVBoxLayout* m_layout;
};

class NExpanderPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightHeaderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHeaderColor)
    Q_PROPERTY_CREATE_D(int, HeaderHeight)
    Q_PROPERTY_CREATE_D(int, ContentPadding)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)

  public:
    explicit NExpanderPrivate(QObject* parent = nullptr);
    ~NExpanderPrivate();
    Q_D_CREATE(NExpander)

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    bool _expanded{false};
    bool _isAnimating{false};

    NExpanderHeaderWidget*  _headerWidget{nullptr};
    NExpanderContentWidget* _contentWidget{nullptr};
    QVariantAnimation*      _heightAnimation{nullptr};
    QVariantAnimation*      _chevronAnimation{nullptr};

    QColor getCurrentColor(const QColor& lightColor, const QColor& darkColor) const {
        return _isDark ? darkColor : lightColor;
    }
};

#endif // QTNATIVEUI_NEXPANDER_P_H
