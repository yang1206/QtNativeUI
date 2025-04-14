#ifndef NAVIGATION_EXAMPLE_H
#define NAVIGATION_EXAMPLE_H

#include <QWidget>

class NScrollArea;
class NavigationExample : public QWidget {
    Q_OBJECT
  public:
    explicit NavigationExample(QWidget* parent = nullptr);
    ~NavigationExample() override;

  private:
    void     initUI();
    QWidget* createTabBars();
    QWidget* createTabWidgets();

  private:
    NScrollArea* m_scrollArea;
};

#endif // NAVIGATION_EXAMPLE_H