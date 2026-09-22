#include "nitemeditor_p.h"

#include <QMetaType>
#include <QStyledItemDelegate>

#include "QtNativeUI/NComboBox.h"
#include "QtNativeUI/NDoubleSpinBox.h"
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NSpinBox.h"

namespace NItemEditor {

QWidget* createLineEdit(QWidget* parent, int borderRadius, const QString& text) {
    auto* editor = new NLineEdit(parent);
    editor->setBorderWidth(1);
    editor->setBorderRadius(borderRadius);
    if (!text.isEmpty())
        editor->setText(text);
    return editor;
}

QWidget* createEditor(QWidget* parent, const QModelIndex& index, int borderRadius) {
    if (!parent || !index.isValid() || !(index.flags() & Qt::ItemIsEditable))
        return nullptr;
    QVariant value = index.data(Qt::EditRole);
    if (!value.isValid())
        value = index.data(Qt::DisplayRole);
    const QMetaType meta = value.metaType();
    switch (meta.id()) {
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            return new NSpinBox(parent);
        case QMetaType::Double:
        case QMetaType::Float:
            return new NDoubleSpinBox(parent);
        default:
            break;
    }
    QVariant choices = index.data(NListViewType::ComboChoicesRole);
    if (!choices.isValid())
        choices = index.data(Qt::UserRole);
    if (choices.metaType().id() == QMetaType::QStringList) {
        auto* combo = new NComboBox(parent);
        combo->addItems(choices.toStringList());
        return combo;
    }
    return createLineEdit(parent, borderRadius, value.toString());
}

QMargins tableCellMargins(int cellHorizontalPadding) {
    const int pad = cellHorizontalPadding;
    return QMargins(pad - 4, 3, pad - 4, 3);
}

QMargins listCellMargins(const QModelIndex& index) {
    int left = 14;
    if (index.flags() & Qt::ItemIsUserCheckable)
        left += 26;
    const QVariant decoration = index.data(Qt::DecorationRole);
    if (decoration.isValid() && !decoration.isNull())
        left += 28;
    return QMargins(left, 3, 12, 3);
}

QMargins treeCellMargins(const QModelIndex& index) {
    int left = 12;
    if (index.flags() & Qt::ItemIsUserCheckable)
        left += 26;
    left += 24;
    const QVariant decoration = index.data(Qt::DecorationRole);
    if (decoration.isValid() && !decoration.isNull())
        left += 24;
    return QMargins(left, 3, 8, 3);
}

void placeEditor(QWidget* editor, const QRect& cellRect, const QMargins& margins) {
    if (!editor)
        return;
    editor->setGeometry(cellRect.adjusted(margins.left(), margins.top(), -margins.right(), -margins.bottom()));
}

void setEditorData(QWidget* editor, const QModelIndex& index) {
    if (!editor || !index.isValid())
        return;
    const QVariant value = index.data(Qt::EditRole);
    if (auto* line = qobject_cast<NLineEdit*>(editor)) {
        line->setText(value.toString());
        return;
    }
    if (auto* spin = qobject_cast<NSpinBox*>(editor)) {
        spin->setValue(value.toInt());
        return;
    }
    if (auto* spin = qobject_cast<NDoubleSpinBox*>(editor)) {
        spin->setValue(value.toDouble());
        return;
    }
    if (auto* combo = qobject_cast<NComboBox*>(editor)) {
        const int row = combo->findText(value.toString());
        combo->setCurrentIndex(row >= 0 ? row : 0);
        return;
    }
    QStyledItemDelegate delegate;
    delegate.setEditorData(editor, index);
}

void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) {
    if (!editor || !model || !index.isValid())
        return;
    if (auto* line = qobject_cast<NLineEdit*>(editor)) {
        model->setData(index, line->text(), Qt::EditRole);
        return;
    }
    if (auto* spin = qobject_cast<NSpinBox*>(editor)) {
        model->setData(index, spin->value(), Qt::EditRole);
        return;
    }
    if (auto* spin = qobject_cast<NDoubleSpinBox*>(editor)) {
        model->setData(index, spin->value(), Qt::EditRole);
        return;
    }
    if (auto* combo = qobject_cast<NComboBox*>(editor)) {
        model->setData(index, combo->currentText(), Qt::EditRole);
        return;
    }
    QStyledItemDelegate delegate;
    delegate.setModelData(editor, model, index);
}

} // namespace NItemEditor
