#ifndef QTNATIVEUI_NHYPERLINKBUTTON_P_H
#define QTNATIVEUI_NHYPERLINKBUTTON_P_H

#include <QtCore/qurl.h>
#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NHyperlinkButton.h>

class NHyperlinkButtonPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(QColor, DisabledColor)
    Q_PROPERTY_CREATE_D(QColor, BackgroundHoverColor)
    Q_PROPERTY_CREATE_D(QColor, BackgroundPressColor)

public:
    explicit NHyperlinkButtonPrivate(QObject* parent = nullptr);
    ~NHyperlinkButtonPrivate();
    Q_D_CREATE(NHyperlinkButton)

private:
    bool _isPressed{false};
    bool _isHovered{false};
    bool _isDark;
    QUrl _url;
    int _pBorderRadius;
};

#endif // QTNATIVEUI_NHYPERLINKBUTTON_P_H