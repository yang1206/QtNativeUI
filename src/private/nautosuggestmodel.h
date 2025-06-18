#ifndef NSUGGESTMODEL_H
#define NSUGGESTMODEL_H

#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <QVector>

#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NAutoSuggestion;

class NAutoSuggestModel : public QAbstractListModel {
    Q_OBJECT

  public:
    explicit NAutoSuggestModel(QObject* parent = nullptr);
    ~NAutoSuggestModel();

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void             setSuggestions(const QVector<NAutoSuggestion*>& suggestions);
    void             clearSuggestions();
    NAutoSuggestion* getSuggestion(int row) const;

  private:
    QVector<NAutoSuggestion*> _suggestions;
};

class NAutoSuggestDelegate : public QStyledItemDelegate {
    Q_OBJECT

  public:
    explicit NAutoSuggestDelegate(QObject* parent = nullptr);
    ~NAutoSuggestDelegate();

    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

  private:
    NThemeType::ThemeMode _themeMode{NThemeType::Light};
};

#endif // NSUGGESTMODEL_H