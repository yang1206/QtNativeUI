#ifndef ICONEXAMPLE_H
#define ICONEXAMPLE_H

#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QScrollArea>
#include <QTimer>
#include <QWidget>
#include "QtNativeUI/NIconEnums.h"
#include "QtNativeUI/NPushButton.h"

class IconButton : public NPushButton {
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
    void onSearchTextChanged();
    void onIconClicked();
    void applyFilter();
    void onIconTypeChanged(int id);

  private:
    void initUI();
    void loadIcons();
    void loadIconsForPage(int page);
    void copyIconCode(IconButton* button);

    struct IconInfo {
        QString name;
        quint32 code;
        bool    isFilled;
    };

    enum IconFilterType { All = 0, RegularOnly = 1, FilledOnly = 2 };

    QLineEdit*   m_searchEdit;
    QScrollArea* m_scrollArea;
    QWidget*     m_contentWidget;
    QGridLayout* m_gridLayout;
    QLabel*      m_statusLabel;
    NPushButton* m_prevButton;
    NPushButton* m_nextButton;
    QLabel*      m_pageLabel;
    QTimer*      m_searchTimer;

    // 图标类型筛选
    QButtonGroup* m_iconTypeGroup;
    QRadioButton* m_allTypesRadio;
    QRadioButton* m_regularRadio;
    QRadioButton* m_filledRadio;

    QList<IconInfo>        m_allIcons;
    QList<IconInfo>        m_filteredIcons;
    QMap<int, IconButton*> m_currentButtons; // 跟踪当前页面上的按钮

    QString          m_searchText;
    int              m_currentPage    = 0;
    IconFilterType   m_iconFilterType = All;
    static const int ICONS_PER_PAGE   = 104; // 8行x13列 = 104个图标
    static const int GRID_COLUMNS     = 8;   // 固定8列显示
};

#endif // ICONEXAMPLE_H