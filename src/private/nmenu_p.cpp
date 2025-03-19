#include "nmenu_p.h"

#include <qstyleoption.h>

#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

NMenuPrivate::Style::Style(NMenuPrivate* parent, QStyle* style) : QProxyStyle(style), d(parent) {}

void NMenuPrivate::Style::drawControl(ControlElement      element,
                                      const QStyleOption* option,
                                      QPainter*           painter,
                                      const QWidget*      widget) const {
    if (element == CE_MenuItem) {
        QProxyStyle::drawControl(element, option, painter, widget);
    } else if (element == CE_MenuBarItem) {
        // 自定义菜单栏项的绘制
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    } else if (element == CE_MenuScroller) {
        // 自定义菜单滚动条的绘制
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    } else if (element == CE_MenuTearoff) {
        // 自定义菜单可拖动部分的绘制
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    } else if (element == CE_MenuEmptyArea) {
        // 自定义菜单空白区域的绘制
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }
}

void NMenuPrivate::Style::drawPrimitive(PrimitiveElement    element,
                                        const QStyleOption* option,
                                        QPainter*           painter,
                                        const QWidget*      widget) const {
    if (element == PE_PanelMenu) {
        // 不绘制菜单面板，由NMenu::paintEvent处理
        return;
    } else if (element == PE_IndicatorMenuCheckMark) {
        // 不绘制菜单勾选标记，由drawControl处理
        return;
    } else if (element == PE_FrameMenu) {
        // 不绘制菜单边框，由NMenu::paintEvent处理
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int NMenuPrivate::Style::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
    const NMenu* menu = qobject_cast<const NMenu*>(widget);

    if (menu) {
        const NMenuPrivate* d = menu->d_func();

        switch (metric) {
            case QStyle::PM_MenuPanelWidth:
                return 0; // 菜单面板宽度
            case QStyle::PM_MenuHMargin:
                return d->_itemPadding; // 菜单水平边距
            case QStyle::PM_MenuVMargin:
                return 4; // 菜单垂直边距
            case QStyle::PM_MenuScrollerHeight:
                return 16; // 菜单滚动条高度
            case QStyle::PM_SubMenuOverlap:
                return -2; // 子菜单重叠量
            case QStyle::PM_MenuDesktopFrameWidth:
                return 0; // 桌面菜单框架宽度
            case QStyle::PM_MenuTearoffHeight:
                return 0; // 菜单可拖动部分高度
        }
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

NMenuPrivate::NMenuPrivate(QObject* parent) : QObject(parent) {}

NMenuPrivate::~NMenuPrivate() {}

void NMenuPrivate::updateThemeColors() { _isDark = nTheme->isDarkMode(); }