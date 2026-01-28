#ifndef ICONSPAGE_H
#define ICONSPAGE_H

#include "../BasePage.h"
#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QtNativeUI/NIconEnums.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NRadioButton.h>

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

class IconsPage : public BasePage {
    Q_OBJECT
  public:
    explicit IconsPage(QWidget* parent = nullptr);

  private slots:
    void onSearchTextChanged();
    void onIconClicked();
    void applyFilter();
    void onIconTypeChanged(int id);

  private:
    void loadIcons();
    void loadIconsForPage(int page);
    void copyIconCode(IconButton* button);

    struct IconInfo {
        QString name;
        quint32 code;
        bool    isFilled;
    };

    enum IconFilterType { All = 0, RegularOnly = 1, FilledOnly = 2 };

    NLineEdit*   m_searchEdit;
    QWidget*     m_contentWidget;
    QGridLayout* m_gridLayout;
    QLabel*      m_statusLabel;
    NPushButton* m_prevButton;
    NPushButton* m_nextButton;
    QLabel*      m_pageLabel;
    QTimer*      m_searchTimer;

    QButtonGroup* m_iconTypeGroup;
    NRadioButton* m_allTypesRadio;
    NRadioButton* m_regularRadio;
    NRadioButton* m_filledRadio;

    QList<IconInfo>        m_allIcons;
    QList<IconInfo>        m_filteredIcons;
    QMap<int, IconButton*> m_currentButtons;

    QString          m_searchText;
    int              m_currentPage    = 0;
    IconFilterType   m_iconFilterType = All;
    static const int ICONS_PER_PAGE   = 104;
    static const int GRID_COLUMNS     = 8;
};

#endif // ICONSPAGE_H