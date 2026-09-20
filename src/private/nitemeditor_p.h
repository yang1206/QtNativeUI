#ifndef NITEMEDITOR_P_H
#define NITEMEDITOR_P_H

#include <QAbstractItemModel>
#include <QMargins>
#include <QModelIndex>
#include <QRect>
#include <QString>

class QWidget;

namespace NItemEditor {
QWidget* createLineEdit(QWidget* parent, int borderRadius, const QString& text = QString());
QWidget* createEditor(QWidget* parent, const QModelIndex& index, int borderRadius);
QMargins tableCellMargins(int cellHorizontalPadding);
QMargins listCellMargins(const QModelIndex& index);
void     placeEditor(QWidget* editor, const QRect& cellRect, const QMargins& margins);
void     setEditorData(QWidget* editor, const QModelIndex& index);
void     setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index);
}

#endif
