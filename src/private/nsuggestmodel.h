#ifndef NSUGGESTMODEL_H
#define NSUGGESTMODEL_H

#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <QVector>

#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NSuggestion;

class NSuggestModel : public QAbstractListModel {
    Q_OBJECT

  public:
    explicit NSuggestModel(QObject* parent = nullptr);
    ~NSuggestModel();

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void         setSuggestions(const QVector<NSuggestion*>& suggestions);
    void         clearSuggestions();
    NSuggestion* getSuggestion(int row) const;

  private:
    QVector<NSuggestion*> _suggestions;
};

class NSuggestDelegate : public QStyledItemDelegate {
    Q_OBJECT

  public:
    explicit NSuggestDelegate(QObject* parent = nullptr);
    ~NSuggestDelegate();

    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

  private:
    NThemeType::ThemeMode _themeMode{NThemeType::Light};
};

#endif // NSUGGESTMODEL_H