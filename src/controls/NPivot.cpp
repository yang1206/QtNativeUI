#include "QtNativeUI/NPivot.h"
#include "../private/npivot_p.h"
#include "QtNativeUI/NColor.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>

Q_PROPERTY_CREATE_Q_CPP(NPivot, int, ItemSpacing)
Q_PROPERTY_CREATE_Q_CPP(NPivot, int, IconSpacing)
Q_PROPERTY_CREATE_Q_CPP(NPivot, int, IndicatorHeight)
Q_PROPERTY_CREATE_Q_CPP(NPivot, int, IndicatorRadius)
Q_PROPERTY_CREATE_Q_CPP(NPivot, int, AnimationDuration)
Q_PROPERTY_CREATE_Q_CPP(NPivot, int, BadgeSize)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, LightTextColorSelected)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, DarkTextColorSelected)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, LightTextColorHover)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, DarkTextColorHover)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, LightTextColorDisabled)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, DarkTextColorDisabled)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, LightIndicatorColor)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, DarkIndicatorColor)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, LightBadgeColor)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, DarkBadgeColor)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, LightBadgeTextColor)
Q_PROPERTY_CREATE_Q_CPP(NPivot, QColor, DarkBadgeTextColor)

NPivot::NPivot(QWidget* parent) : QWidget(parent), d_ptr(new NPivotPrivate()) { init(); }

NPivot::~NPivot() {}

void NPivot::init() {
    Q_D(NPivot);
    d->q_ptr = this;

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    d->_pItemSpacing       = 24;
    d->_pIconSpacing       = 6;
    d->_pIndicatorHeight   = 3;
    d->_pIndicatorRadius   = 2;
    d->_pAnimationDuration = 250;
    d->_pBadgeSize         = 16;
    d->_pAlignment         = NPivotType::Left;

    d->_pLightTextColor         = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkTextColor          = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightTextColorSelected = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColorSelected  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightTextColorHover    = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColorHover     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightTextColorDisabled = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkTextColorDisabled  = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);
    d->_pLightIndicatorColor    = nTheme->accentColor().defaultBrushFor(false);
    d->_pDarkIndicatorColor     = nTheme->accentColor().defaultBrushFor(true);
    d->_pLightBadgeColor        = nTheme->accentColor().defaultBrushFor(false);
    d->_pDarkBadgeColor         = nTheme->accentColor().defaultBrushFor(true);
    d->_pLightBadgeTextColor    = QColor(255, 255, 255);
    d->_pDarkBadgeTextColor     = QColor(0, 0, 0);

    d->_posAnimation = new QPropertyAnimation(this, "indicatorX", this);
    d->_posAnimation->setDuration(d->_pAnimationDuration);
    d->_posAnimation->setEasingCurve(QEasingCurve::OutCubic);

    d->_widthAnimation = new QPropertyAnimation(this, "indicatorWidth", this);
    d->_widthAnimation->setDuration(d->_pAnimationDuration);
    d->_widthAnimation->setEasingCurve(QEasingCurve::OutCubic);

    setObjectName("NPivot");
    setAttribute(Qt::WA_Hover, true);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setMinimumHeight(32);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NPivot);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor& color) {
        Q_D(NPivot);
        d->_pLightIndicatorColor = color.defaultBrushFor(false);
        d->_pDarkIndicatorColor  = color.defaultBrushFor(true);
        d->_pLightBadgeColor     = color.defaultBrushFor(false);
        d->_pDarkBadgeColor      = color.defaultBrushFor(true);
        update();
    });
}

int NPivot::addItem(const QString& text) {
    return insertItem(count(), text);
}

int NPivot::addItem(const QIcon& icon, const QString& text) {
    return insertItem(count(), icon, text);
}

int NPivot::addItem(NRegularIconType::Icon icon, const QString& text, int iconSize) {
    QIcon qIcon = nIcon->fromRegular(icon);
    int index = insertItem(count(), qIcon, text);
    if (index >= 0) {
        Q_D(NPivot);
        d->_items[index].iconSize = iconSize;
    }
    return index;
}

int NPivot::addItem(NFilledIconType::Icon icon, const QString& text, int iconSize) {
    QIcon qIcon = nIcon->fromFilled(icon);
    int index = insertItem(count(), qIcon, text);
    if (index >= 0) {
        Q_D(NPivot);
        d->_items[index].iconSize = iconSize;
    }
    return index;
}

int NPivot::insertItem(int index, const QIcon& icon, const QString& text) {
    Q_D(NPivot);

    index = qBound(0, index, d->_items.size());

    NPivotItem item;
    item.text    = text;
    item.icon    = icon;
    item.enabled = true;

    d->_items.insert(index, item);

    if (d->_currentIndex < 0) {
        d->_currentIndex = 0;
        QRect rect       = itemRect(0);
        d->_indicatorX     = rect.x();
        d->_indicatorWidth = rect.width();
        emit currentIndexChanged(0);
    } else if (index <= d->_currentIndex) {
        d->_currentIndex++;
    }

    updateGeometry();
    update();

    return index;
}

int NPivot::insertItem(int index, const QString& text) {
    Q_D(NPivot);

    index = qBound(0, index, d->_items.size());

    NPivotItem item;
    item.text    = text;
    item.enabled = true;

    d->_items.insert(index, item);

    if (d->_currentIndex < 0) {
        d->_currentIndex = 0;
        QRect rect       = itemRect(0);
        d->_indicatorX     = rect.x();
        d->_indicatorWidth = rect.width();
        emit currentIndexChanged(0);
    } else if (index <= d->_currentIndex) {
        d->_currentIndex++;
    }

    updateGeometry();
    update();

    return index;
}

void NPivot::removeItem(int index) {
    Q_D(NPivot);

    if (index < 0 || index >= d->_items.size()) {
        return;
    }

    d->_items.removeAt(index);

    if (d->_items.isEmpty()) {
        d->_currentIndex   = -1;
        d->_indicatorX     = 0;
        d->_indicatorWidth = 0;
    } else if (index < d->_currentIndex) {
        d->_currentIndex--;
    } else if (index == d->_currentIndex) {
        d->_currentIndex = qMin(d->_currentIndex, d->_items.size() - 1);
        QRect rect       = itemRect(d->_currentIndex);
        d->_indicatorX     = rect.x();
        d->_indicatorWidth = rect.width();
        emit currentIndexChanged(d->_currentIndex);
    }

    updateGeometry();
    update();
}

void NPivot::clear() {
    Q_D(NPivot);

    d->_items.clear();
    d->_currentIndex   = -1;
    d->_hoverIndex     = -1;
    d->_indicatorX     = 0;
    d->_indicatorWidth = 0;

    updateGeometry();
    update();
}

int NPivot::count() const {
    Q_D(const NPivot);
    return d->_items.size();
}

int NPivot::currentIndex() const {
    Q_D(const NPivot);
    return d->_currentIndex;
}

QString NPivot::itemText(int index) const {
    Q_D(const NPivot);
    if (index < 0 || index >= d->_items.size()) {
        return QString();
    }
    return d->_items[index].text;
}

void NPivot::setItemText(int index, const QString& text) {
    Q_D(NPivot);
    if (index < 0 || index >= d->_items.size()) {
        return;
    }
    d->_items[index].text = text;

    if (index == d->_currentIndex) {
        QRect rect         = itemRect(index);
        d->_indicatorX     = rect.x();
        d->_indicatorWidth = rect.width();
    }

    updateGeometry();
    update();
}

void NPivot::setItemIcon(int index, const QIcon& icon) {
    Q_D(NPivot);
    if (index < 0 || index >= d->_items.size()) {
        return;
    }
    d->_items[index].icon = icon;

    if (index == d->_currentIndex) {
        QRect rect         = itemRect(index);
        d->_indicatorX     = rect.x();
        d->_indicatorWidth = rect.width();
    }

    updateGeometry();
    update();
}

void NPivot::setItemIcon(int index, NRegularIconType::Icon icon, int size) {
    setItemIcon(index, nIcon->fromRegular(icon));
    Q_D(NPivot);
    if (index >= 0 && index < d->_items.size()) {
        d->_items[index].iconSize = size;
    }
}

void NPivot::setItemIcon(int index, NFilledIconType::Icon icon, int size) {
    setItemIcon(index, nIcon->fromFilled(icon));
    Q_D(NPivot);
    if (index >= 0 && index < d->_items.size()) {
        d->_items[index].iconSize = size;
    }
}

QIcon NPivot::itemIcon(int index) const {
    Q_D(const NPivot);
    if (index < 0 || index >= d->_items.size()) {
        return QIcon();
    }
    return d->_items[index].icon;
}

void NPivot::setItemBadge(int index, int count) {
    Q_D(NPivot);
    if (index < 0 || index >= d->_items.size()) {
        return;
    }
    d->_items[index].badge = count;
    d->_items[index].showBadge = count > 0;
    update();
}

int NPivot::itemBadge(int index) const {
    Q_D(const NPivot);
    if (index < 0 || index >= d->_items.size()) {
        return 0;
    }
    return d->_items[index].badge;
}

void NPivot::clearItemBadge(int index) {
    Q_D(NPivot);
    if (index < 0 || index >= d->_items.size()) {
        return;
    }
    d->_items[index].badge = 0;
    d->_items[index].showBadge = false;
    update();
}

void NPivot::setItemEnabled(int index, bool enabled) {
    Q_D(NPivot);
    if (index < 0 || index >= d->_items.size()) {
        return;
    }
    d->_items[index].enabled = enabled;
    update();
}

bool NPivot::isItemEnabled(int index) const {
    Q_D(const NPivot);
    if (index < 0 || index >= d->_items.size()) {
        return false;
    }
    return d->_items[index].enabled;
}

void NPivot::setItemFont(const QFont& font) {
    Q_D(NPivot);
    d->_itemFont = font;
    if (d->_currentIndex >= 0) {
        QRect rect         = itemRect(d->_currentIndex);
        d->_indicatorX     = rect.x();
        d->_indicatorWidth = rect.width();
    }
    updateGeometry();
    update();
}

QFont NPivot::itemFont() const {
    Q_D(const NPivot);
    return d->_itemFont;
}

void NPivot::setSelectedItemFont(const QFont& font) {
    Q_D(NPivot);
    d->_selectedItemFont       = font;
    d->_useCustomSelectedFont = true;
    if (d->_currentIndex >= 0) {
        QRect rect         = itemRect(d->_currentIndex);
        d->_indicatorX     = rect.x();
        d->_indicatorWidth = rect.width();
    }
    updateGeometry();
    update();
}

QFont NPivot::selectedItemFont() const {
    Q_D(const NPivot);
    if (d->_useCustomSelectedFont) {
        return d->_selectedItemFont;
    }
    QFont f = d->_itemFont;
    f.setBold(true);
    return f;
}

void NPivot::addRightWidget(QWidget* widget, int spacing) {
    Q_D(NPivot);
    if (!widget) return;

    for (const auto& rw : d->_rightWidgets) {
        if (rw.widget == widget) return;
    }

    NPivotRightWidget rw;
    rw.widget  = widget;
    rw.spacing = spacing;

    widget->setParent(this);
    widget->show();

    d->_rightWidgets.append(rw);
    updateRightWidgetsPosition();
    update();
}

void NPivot::removeRightWidget(QWidget* widget) {
    Q_D(NPivot);
    for (int i = 0; i < d->_rightWidgets.size(); ++i) {
        if (d->_rightWidgets[i].widget == widget) {
            d->_rightWidgets.removeAt(i);
            widget->setParent(nullptr);
            updateRightWidgetsPosition();
            update();
            return;
        }
    }
}

void NPivot::clearRightWidgets() {
    Q_D(NPivot);
    for (auto& rw : d->_rightWidgets) {
        if (rw.widget) {
            rw.widget->setParent(nullptr);
        }
    }
    d->_rightWidgets.clear();
    update();
}

QList<QWidget*> NPivot::rightWidgets() const {
    Q_D(const NPivot);
    QList<QWidget*> list;
    for (const auto& rw : d->_rightWidgets) {
        list.append(rw.widget);
    }
    return list;
}

qreal NPivot::indicatorX() const {
    Q_D(const NPivot);
    return d->_indicatorX;
}

void NPivot::setIndicatorX(qreal x) {
    Q_D(NPivot);
    if (d->_indicatorX != x) {
        d->_indicatorX = x;
        update();
    }
}

qreal NPivot::indicatorWidth() const {
    Q_D(const NPivot);
    return d->_indicatorWidth;
}

void NPivot::setIndicatorWidth(qreal width) {
    Q_D(NPivot);
    if (d->_indicatorWidth != width) {
        d->_indicatorWidth = width;
        update();
    }
}

void NPivot::setAlignment(NPivotType::Alignment alignment) {
    Q_D(NPivot);
    if (d->_pAlignment != alignment) {
        d->_pAlignment = alignment;
        
        // 对齐方式改变时，重新计算指示器位置
        if (d->_currentIndex >= 0) {
            QRect rect = itemRect(d->_currentIndex);
            d->_indicatorX = rect.x();
            d->_indicatorWidth = rect.width();
        }
        
        update();
    }
}

NPivotType::Alignment NPivot::alignment() const {
    Q_D(const NPivot);
    return d->_pAlignment;
}

void NPivot::setCurrentIndex(int index) {
    Q_D(NPivot);

    if (index < 0 || index >= d->_items.size()) {
        return;
    }

    if (!d->_items[index].enabled) {
        return;
    }

    if (d->_currentIndex == index) {
        return;
    }

    d->_currentIndex = index;
    startIndicatorAnimation(index);

    emit currentIndexChanged(index);
    update();
}

QSize NPivot::sizeHint() const {
    Q_D(const NPivot);

    int totalWidth = 0;
    for (int i = 0; i < d->_items.size(); ++i) {
        totalWidth += itemWidth(i);
        if (i > 0) {
            totalWidth += d->_pItemSpacing;
        }
    }

    QFont        normalFont = d->_itemFont.pointSize() > 0 ? d->_itemFont : font();
    QFontMetrics fm(normalFont);
    int          height = fm.height() + d->_pIndicatorHeight + 8;

    return QSize(totalWidth, height);
}

QSize NPivot::minimumSizeHint() const {
    return sizeHint();
}

void NPivot::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    Q_D(NPivot);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QFont normalFont   = d->_itemFont.pointSize() > 0 ? d->_itemFont : font();
    QFont selectedFont = selectedItemFont();

    QFontMetrics fm(normalFont);
    int          textY = (height() - d->_pIndicatorHeight - fm.height()) / 2 + fm.ascent();

    for (int i = 0; i < d->_items.size(); ++i) {
        QRect itemRect = this->itemRect(i);
        int   currentX = itemRect.x();

        // 绘制图标
        if (!d->_items[i].icon.isNull()) {
            int iconSize = d->_items[i].iconSize;
            int iconY = (height() - d->_pIndicatorHeight - iconSize) / 2;
            
            QRect iconRect(currentX, iconY, iconSize, iconSize);
            d->_items[i].icon.paint(&painter, iconRect);
            
            currentX += iconSize + d->_pIconSpacing;
        }

        // 绘制文本
        QColor textColor;
        if (!d->_items[i].enabled) {
            textColor = d->getCurrentColor(d->_pLightTextColorDisabled, d->_pDarkTextColorDisabled);
        } else if (i == d->_currentIndex) {
            textColor = d->getCurrentColor(d->_pLightTextColorSelected, d->_pDarkTextColorSelected);
        } else if (i == d->_hoverIndex) {
            textColor = d->getCurrentColor(d->_pLightTextColorHover, d->_pDarkTextColorHover);
        } else {
            textColor = d->getCurrentColor(d->_pLightTextColor, d->_pDarkTextColor);
        }

        painter.setPen(textColor);
        painter.setFont(i == d->_currentIndex ? selectedFont : normalFont);
        painter.drawText(currentX, textY, d->_items[i].text);

        // 绘制徽章
        if (d->_items[i].showBadge) {
            drawBadge(&painter, itemRect, d->_items[i].badge);
        }
    }

    // 绘制指示器
    if (d->_currentIndex >= 0 && d->_indicatorWidth > 0) {
        QColor indicatorColor = d->getCurrentColor(d->_pLightIndicatorColor, d->_pDarkIndicatorColor);

        painter.setPen(Qt::NoPen);
        painter.setBrush(indicatorColor);

        QRectF indicatorRect(d->_indicatorX, height() - d->_pIndicatorHeight, d->_indicatorWidth, d->_pIndicatorHeight);
        painter.drawRoundedRect(indicatorRect, d->_pIndicatorRadius, d->_pIndicatorRadius);
    }
}

void NPivot::mousePressEvent(QMouseEvent* event) {
    Q_D(NPivot);

    if (event->button() == Qt::LeftButton) {
        int rightAreaStart = width() - d->rightWidgetsTotalWidth();
        if (event->pos().x() >= rightAreaStart) {
            QWidget::mousePressEvent(event);
            return;
        }

        int index = itemAtPos(event->pos());
        if (index >= 0 && d->_items[index].enabled) {
            setCurrentIndex(index);
        }
    }

    QWidget::mousePressEvent(event);
}

void NPivot::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NPivot);

    int index = itemAtPos(event->pos());
    if (index != d->_hoverIndex) {
        d->_hoverIndex = index;
        update();
    }

    QWidget::mouseMoveEvent(event);
}

void NPivot::leaveEvent(QEvent* event) {
    Q_D(NPivot);

    d->_hoverIndex = -1;
    update();

    QWidget::leaveEvent(event);
}

void NPivot::resizeEvent(QResizeEvent* event) {
    Q_D(NPivot);

    if (d->_currentIndex >= 0) {
        QRect rect         = itemRect(d->_currentIndex);
        d->_indicatorX     = rect.x();
        d->_indicatorWidth = rect.width();
    }

    updateRightWidgetsPosition();
    QWidget::resizeEvent(event);
}

int NPivot::itemAtPos(const QPoint& pos) const {
    Q_D(const NPivot);

    int rightAreaStart = width() - d->rightWidgetsTotalWidth();
    if (pos.x() >= rightAreaStart) {
        return -1;
    }

    for (int i = 0; i < d->_items.size(); ++i) {
        if (itemRect(i).contains(pos)) {
            return i;
        }
    }
    return -1;
}

int NPivot::itemWidth(int index) const {
    Q_D(const NPivot);

    if (index < 0 || index >= d->_items.size()) {
        return 0;
    }

    QFont        normalFont = d->_itemFont.pointSize() > 0 ? d->_itemFont : font();
    QFontMetrics fm(normalFont);
    int          textWidth = fm.horizontalAdvance(d->_items[index].text);
    int          totalWidth = textWidth;

    // 添加图标宽度
    if (!d->_items[index].icon.isNull()) {
        totalWidth += d->_items[index].iconSize + d->_pIconSpacing;
    }

    // 添加徽章宽度
    if (d->_items[index].showBadge) {
        totalWidth += d->_pBadgeSize + 4; // 徽章大小 + 间距
    }

    return totalWidth;
}

QRect NPivot::itemRect(int index) const {
    Q_D(const NPivot);

    if (index < 0 || index >= d->_items.size()) {
        return QRect();
    }

    int x = getAlignmentOffset();
    for (int i = 0; i < index; ++i) {
        x += itemWidth(i) + d->_pItemSpacing;
    }

    int w = itemWidth(index);
    return QRect(x, 0, w, height());
}

QRect NPivot::textRect(int index) const {
    Q_D(const NPivot);

    if (index < 0 || index >= d->_items.size()) {
        return QRect();
    }

    int x = getAlignmentOffset();
    for (int i = 0; i < index; ++i) {
        x += itemWidth(i) + d->_pItemSpacing;
    }

    // 如果有图标，文本需要偏移
    if (!d->_items[index].icon.isNull()) {
        x += d->_items[index].iconSize + d->_pIconSpacing;
    }

    QFont        normalFont = d->_itemFont.pointSize() > 0 ? d->_itemFont : font();
    QFontMetrics fm(normalFont);
    int          w = fm.horizontalAdvance(d->_items[index].text);

    return QRect(x, 0, w, fm.height());
}

void NPivot::startIndicatorAnimation(int toIndex) {
    Q_D(NPivot);

    QRect rect = itemRect(toIndex);  // 使用 itemRect 而不是 textRect

    d->_posAnimation->stop();
    d->_widthAnimation->stop();

    d->_posAnimation->setDuration(d->_pAnimationDuration);
    d->_widthAnimation->setDuration(d->_pAnimationDuration);

    d->_posAnimation->setStartValue(d->_indicatorX);
    d->_posAnimation->setEndValue(static_cast<qreal>(rect.x()));

    d->_widthAnimation->setStartValue(d->_indicatorWidth);
    d->_widthAnimation->setEndValue(static_cast<qreal>(rect.width()));

    d->_posAnimation->start();
    d->_widthAnimation->start();
}

void NPivot::updateRightWidgetsPosition() {
    Q_D(NPivot);

    int x = width();
    int y = (height() - d->_pIndicatorHeight) / 2;

    for (int i = d->_rightWidgets.size() - 1; i >= 0; --i) {
        auto& rw = d->_rightWidgets[i];
        if (rw.widget) {
            QSize sz = rw.widget->sizeHint();
            x -= sz.width();
            rw.widget->setGeometry(x, y - sz.height() / 2, sz.width(), sz.height());
            x -= rw.spacing;
        }
    }
}

void NPivot::drawBadge(QPainter* painter, const QRect& itemRect, int badge) {
    Q_D(NPivot);
    
    if (badge <= 0) return;

    QString badgeText = badge > 99 ? "99+" : QString::number(badge);
    QFont badgeFont = font();
    badgeFont.setPointSize(qMax(8, badgeFont.pointSize() - 2));
    badgeFont.setBold(true);
    
    QFontMetrics badgeFm(badgeFont);
    int textWidth = badgeFm.horizontalAdvance(badgeText);
    int badgeWidth = qMax(d->_pBadgeSize, textWidth + 6);
    
    // 徽章位置在项目右上角
    int badgeX = itemRect.right() - badgeWidth / 2;
    int badgeY = itemRect.top() + 2;
    
    QRect badgeRect(badgeX, badgeY, badgeWidth, d->_pBadgeSize);
    
    // 绘制徽章背景
    QColor badgeColor = d->getCurrentColor(d->_pLightBadgeColor, d->_pDarkBadgeColor);
    painter->setPen(Qt::NoPen);
    painter->setBrush(badgeColor);
    painter->drawRoundedRect(badgeRect, d->_pBadgeSize / 2, d->_pBadgeSize / 2);
    
    // 绘制徽章文本
    QColor badgeTextColor = d->getCurrentColor(d->_pLightBadgeTextColor, d->_pDarkBadgeTextColor);
    painter->setPen(badgeTextColor);
    painter->setFont(badgeFont);
    painter->drawText(badgeRect, Qt::AlignCenter, badgeText);
}

void NPivot::keyPressEvent(QKeyEvent* event) {
    Q_D(NPivot);
    
    if (d->_items.isEmpty()) {
        QWidget::keyPressEvent(event);
        return;
    }
    
    int newIndex = d->_currentIndex;
    
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Up:
        // 向前导航
        do {
            newIndex = (newIndex - 1 + d->_items.size()) % d->_items.size();
        } while (!d->_items[newIndex].enabled && newIndex != d->_currentIndex);
        break;
        
    case Qt::Key_Right:
    case Qt::Key_Down:
        // 向后导航
        do {
            newIndex = (newIndex + 1) % d->_items.size();
        } while (!d->_items[newIndex].enabled && newIndex != d->_currentIndex);
        break;
        
    case Qt::Key_Home:
        // 跳到第一个可用项
        newIndex = 0;
        while (newIndex < d->_items.size() && !d->_items[newIndex].enabled) {
            newIndex++;
        }
        if (newIndex >= d->_items.size()) newIndex = d->_currentIndex;
        break;
        
    case Qt::Key_End:
        // 跳到最后一个可用项
        newIndex = d->_items.size() - 1;
        while (newIndex >= 0 && !d->_items[newIndex].enabled) {
            newIndex--;
        }
        if (newIndex < 0) newIndex = d->_currentIndex;
        break;
        
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Space:
        // 激活当前项
        if (d->_currentIndex >= 0 && d->_items[d->_currentIndex].enabled) {
            emit currentIndexChanged(d->_currentIndex);
        }
        event->accept();
        return;
        
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    
    if (newIndex != d->_currentIndex) {
        setCurrentIndex(newIndex);
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void NPivot::focusInEvent(QFocusEvent* event) {
    Q_D(NPivot);
    update();
    QWidget::focusInEvent(event);
}

void NPivot::focusOutEvent(QFocusEvent* event) {
    Q_D(NPivot);
    update();
    QWidget::focusOutEvent(event);
}

int NPivot::getAlignmentOffset() const {
    Q_D(const NPivot);
    
    if (d->_pAlignment == NPivotType::Left) {
        return 0;
    }
    
    int totalWidth = 0;
    for (int i = 0; i < d->_items.size(); ++i) {
        totalWidth += itemWidth(i);
        if (i > 0) {
            totalWidth += d->_pItemSpacing;
        }
    }
    
    int availableWidth = width() - d->rightWidgetsTotalWidth();
    int offset = 0;
    
    switch (d->_pAlignment) {
    case NPivotType::Center:
        offset = (availableWidth - totalWidth) / 2;
        break;
    case NPivotType::Right:
        offset = availableWidth - totalWidth;
        break;
    case NPivotType::Left:
    default:
        offset = 0;
        break;
    }
    
    return qMax(0, offset);
}
