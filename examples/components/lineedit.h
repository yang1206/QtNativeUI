#ifndef LINEEDIT_EXAMPLE_H
#define LINEEDIT_EXAMPLE_H

#include <QWidget>

class QLineEdit;
class NLineEdit;

class LineEditExample : public QWidget {
    Q_OBJECT

public:
    explicit LineEditExample(QWidget* parent = nullptr);

private:
    void initUI();
};

#endif // LINEEDIT_EXAMPLE_H 