//
// Created by yang1206 on 2022/3/10.
//

#ifndef BUTTON_H
#define BUTTON_H
#include <QWidget>

class NMainWindow;
class NScrollArea;
class ButtonExample : public QWidget {
    Q_OBJECT
  public:
    explicit ButtonExample(QWidget* parent = nullptr);
    void setMainWindow(NMainWindow* mainWindow) { m_mainWindow = mainWindow; }

  private:
    void     initUI();
    QWidget* createPushButtons();
    QWidget* createToggleButtons();
    QWidget* createHyperlinkButtons();
    QWidget* createRadioButtons();
    QWidget* createToolButtons();

    NScrollArea* m_scrollArea;
    NMainWindow* m_mainWindow = nullptr;
};

#endif // BUTTON_H
