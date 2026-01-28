#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QWidget>

class NScrollArea;
class QVBoxLayout;

class BasePage : public QWidget {
    Q_OBJECT

  public:
    explicit BasePage(const QString& title, const QString& description, QWidget* parent = nullptr);

  protected:
    QVBoxLayout* contentLayout() const { return m_contentLayout; }

    void addSection(const QString& sectionTitle, QWidget* sectionWidget);
    void addSection(const QString& sectionTitle, QLayout* sectionLayout);

  private:
    NScrollArea* m_scrollArea;
    QWidget*     m_contentWidget;
    QVBoxLayout* m_contentLayout;
};

#endif // BASEPAGE_H
