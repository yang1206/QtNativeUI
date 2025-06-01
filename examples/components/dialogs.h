#ifndef DIALOGS_EXAMPLE_H
#define DIALOGS_EXAMPLE_H

#include <QWidget>

class NScrollArea;
class DialogsExample : public QWidget {
    Q_OBJECT
  public:
    explicit DialogsExample(QWidget* parent = nullptr);

  private:
    void     initUI();
    QWidget* createFlyouts();
    QWidget* createContentDialogs();

    NScrollArea* m_scrollArea;
};

#endif // DIALOGS_EXAMPLE_H