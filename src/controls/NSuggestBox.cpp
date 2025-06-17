#include "QtNativeUI/NSuggestBox.h"
#include "../private//nsuggestmodel.h"
#include "../private/nsuggestbox_p.h"
#include "QtNativeUI/NLineEdit.h"

Q_PROPERTY_CREATE_Q_CPP(NSuggestBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NSuggestBox, Qt::CaseSensitivity, CaseSensitivity)

NSuggestBox::NSuggestBox(QWidget* parent) : QWidget(parent), d_ptr(new NSuggestBoxPrivate()) {
    Q_D(NSuggestBox);
    d->q_ptr             = this;
    d->_pBorderRadius    = 6;
    d->_pCaseSensitivity = Qt::CaseInsensitive;

    d->setupUI();
}

NSuggestBox::~NSuggestBox() {}

void NSuggestBox::setPlaceholderText(const QString& placeholderText) {
    Q_D(NSuggestBox);
    d->_lineEdit->setPlaceholderText(placeholderText);
}

QString NSuggestBox::placeholderText() const {
    Q_D(const NSuggestBox);
    return d->_lineEdit->placeholderText();
}

QString NSuggestBox::addSuggestion(const QString& text, const QVariantMap& data) {
    Q_D(NSuggestBox);
    NSuggestion* suggestion = new NSuggestion(this);
    suggestion->setText(text);
    suggestion->setData(data);
    d->_suggestions.append(suggestion);
    return suggestion->getKey();
}

QString NSuggestBox::addSuggestion(NRegularIconType::Icon icon, const QString& text, const QVariantMap& data) {
    Q_D(NSuggestBox);
    NSuggestion* suggestion = new NSuggestion(this);
    suggestion->setIcon(icon);
    suggestion->setText(text);
    suggestion->setData(data);
    d->_suggestions.append(suggestion);
    return suggestion->getKey();
}

void NSuggestBox::removeSuggestion(const QString& key) {
    Q_D(NSuggestBox);
    for (int i = 0; i < d->_suggestions.size(); ++i) {
        if (d->_suggestions[i]->getKey() == key) {
            NSuggestion* suggestion = d->_suggestions.takeAt(i);
            suggestion->deleteLater();
            break;
        }
    }

    // 更新过滤后的列表
    d->onTextChanged(d->_lineEdit->text());
}

void NSuggestBox::removeSuggestion(int index) {
    Q_D(NSuggestBox);
    if (index >= 0 && index < d->_suggestions.size()) {
        NSuggestion* suggestion = d->_suggestions.takeAt(index);
        suggestion->deleteLater();

        // 更新过滤后的列表
        d->onTextChanged(d->_lineEdit->text());
    }
}

void NSuggestBox::clearSuggestions() {
    Q_D(NSuggestBox);
    qDeleteAll(d->_suggestions);
    d->_suggestions.clear();
    d->_filteredSuggestions.clear();
    d->_model->clearSuggestions();
}