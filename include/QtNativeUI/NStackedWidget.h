#ifndef NSTACKEDWIDGET_H
#define NSTACKEDWIDGET_H

#include <QStackedWidget>
#include "NEnums.h"
#include "NTheme.h"
#include "stdafx.h"

class NNavigationAnimationManager;

class QTNATIVEUI_EXPORT NStackedWidget : public QStackedWidget {
    Q_OBJECT

  public:
    explicit NStackedWidget(QWidget* parent = nullptr);
    ~NStackedWidget() override;

    void   setBackGroundColor(QColor background);
    QColor backgroundColor() const { return m_background; }

    void setBorderRadius(int radius);
    int  borderRadius() const { return m_borderRadius; }

    void doPageSwitch(NNavigationType::PageTransitionType transitionType, int targetIndex, bool isRouteBack, int duration = 300);

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    NThemeType::ThemeMode         m_themeMode;
    int                           m_borderRadius{10};
    QColor                        m_background{};
    NNavigationAnimationManager*  m_animationManager{nullptr};
};

#endif // NSTACKEDWIDGET_H
