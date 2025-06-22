//
// Created by Yang1206 on 2025/6/22.
//

#ifndef NSTACKEDWIDGET_H
#define NSTACKEDWIDGET_H

#include <QStackedWidget>
#include "NTheme.h"
class NStackedWidget : public QStackedWidget {
    Q_OBJECT
  public:
    explicit NStackedWidget(QWidget* parent = nullptr);
    ~NStackedWidget() override;

    void   setBackGroundColor(QColor background);
    QColor backgroundColor() { return m_background; }

    void setBorderRadius(int radius);
    int  borderRadius() { return m_borderRadius; }

  protected:
    virtual void paintEvent(QPaintEvent* event) override;

  private:
    NThemeType::ThemeMode m_themeMode;
    int                   m_borderRadius{10};
    QColor                m_background{};
};

#endif // NSTACKEDWIDGET_H
