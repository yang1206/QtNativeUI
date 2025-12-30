#ifndef VIEWEXAMPLE_H
#define VIEWEXAMPLE_H

#include <QWidget>

class NScrollArea;
class NListView;

class ViewExample : public QWidget {
    Q_OBJECT
  public:
    explicit ViewExample(QWidget* parent = nullptr);

  private:
    void initUI();

    QWidget* createListViewSection();

    NScrollArea* m_scrollArea{nullptr};
};

#endif // VIEWEXAMPLE_H
