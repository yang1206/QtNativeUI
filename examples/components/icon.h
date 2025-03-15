#ifndef ICONEXAMPLE_H
#define ICONEXAMPLE_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>
#include "QtNativeUI/NIconEnums.h"

class IconButton : public QPushButton {
    Q_OBJECT
  public:
    explicit IconButton(quint32 iconCode, const QString& name, bool isFilled, QWidget* parent = nullptr);
    QString iconName() const { return m_iconName; }
    quint32 iconCode() const { return m_iconCode; }
    bool    isFilled() const { return m_isFilled; }

  private:
    QString m_iconName;
    quint32 m_iconCode;
    bool    m_isFilled;
};

class IconExample : public QWidget {
    Q_OBJECT
  public:
    explicit IconExample(QWidget* parent = nullptr);

  private slots:
    void onSearchTextChanged(const QString& text);
    void onIconClicked();

  private:
    void initUI();
    // void loadIcons();
    void loadIconsForPage(int page);
    void copyIconCode(IconButton* button);

    struct IconInfo {
        QString name;
        quint32 code;
        bool    isFilled;
    };

    QLineEdit*   m_searchEdit;
    QScrollArea* m_scrollArea;
    QWidget*     m_contentWidget;
    QGridLayout* m_gridLayout;
    QLabel*      m_statusLabel;
    QPushButton* m_prevButton;
    QPushButton* m_nextButton;
    QLabel*      m_pageLabel;

    QList<IconInfo> m_allIcons;
    QList<IconInfo> m_filteredIcons;

    QString          m_searchText;
    int              m_currentPage  = 0;
    static const int ICONS_PER_PAGE = 100;
    static const int GRID_COLUMNS   = 8; // 每行显示的图标数量
};

#endif // ICONEXAMPLE_H