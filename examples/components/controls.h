#ifndef CONTROLS_EXAMPLE_H
#define CONTROLS_EXAMPLE_H

#include <QWidget>

class QScrollArea;

class ControlsExample : public QWidget {
    Q_OBJECT
public:
    explicit ControlsExample(QWidget* parent = nullptr);

private:
    void initUI();
    QWidget* createCheckBoxes();
    QWidget* createToggleSwitches();
    QWidget* createLineEdits();

    QScrollArea* m_scrollArea;
};

#endif // CONTROLS_EXAMPLE_H