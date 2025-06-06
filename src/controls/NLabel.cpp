#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NTheme.h>
#include "../private/nlabel_p.h"

Q_PROPERTY_CREATE_Q_CPP(NLabel, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NLabel, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NLabel, QColor, LightTextDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NLabel, QColor, DarkTextDisabledColor)

NLabel::NLabel(QWidget* parent) : QLabel(parent), d_ptr(new NLabelPrivate(this)) { init(); }

NLabel::NLabel(const QString& text, QWidget* parent) : QLabel(text, parent), d_ptr(new NLabelPrivate(this)) { init(); }

NLabel::NLabel(const QString& text, NLabelType::Type type, QWidget* parent)
    : QLabel(text, parent), d_ptr(new NLabelPrivate(this)) {
    Q_D(NLabel);
    d->_type = type;
    init();
}

NLabel::~NLabel() {}

NLabelType::Type NLabel::type() const {
    Q_D(const NLabel);
    return d->_type;
}

void NLabel::setType(NLabelType::Type type) {
    Q_D(NLabel);
    if (d->_type != type) {
        d->_type = type;
        d->updateFont();
        emit typeChanged();
    }
}

void NLabel::init() {
    Q_D(NLabel);

    // 保存原始字体，以便在设置类型时参考
    d->_originalFont = font();

    // 连接主题变化信号
    connect(nTheme, &NTheme::darkModeChanged, this, [this](bool isDark) {
        Q_D(NLabel);
        d->_isDark = isDark;
        d->updateColor();
    });

    // 初始化样式
    updateStyle();
}

void NLabel::updateStyle() {
    Q_D(NLabel);

    d->updateFont();
    d->updateColor();
}