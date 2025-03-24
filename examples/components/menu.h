#ifndef MENU_EXAMPLE_H
#define MENU_EXAMPLE_H

#include <QWidget>

class MenuExample : public QWidget {
    Q_OBJECT
public:
    explicit MenuExample(QWidget* parent = nullptr);

private:
    void initUI();
};

#endif // MENU_EXAMPLE_H 