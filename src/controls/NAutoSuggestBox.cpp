#include "QtNativeUI/NAutoSuggestBox.h"
#include "../private//nautosuggestmodel.h"
#include "../private/nautosuggestbox_p.h"
#include "QtNativeUI/NLineEdit.h"

Q_PROPERTY_CREATE_Q_CPP(NAutoSuggestBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NAutoSuggestBox, Qt::CaseSensitivity, CaseSensitivity)

NAutoSuggestBox::NAutoSuggestBox(QWidget* parent) : QWidget(parent), d_ptr(new NAutoSuggestBoxPrivate()) {
    Q_D(NAutoSuggestBox);
    d->q_ptr             = this;
    d->_pBorderRadius    = 6;
    d->_pCaseSensitivity = Qt::CaseInsensitive;

    d->setupUI();
}

NAutoSuggestBox::~NAutoSuggestBox() {}

void NAutoSuggestBox::setFilterMode(FilterMode mode) {
    Q_D(NAutoSuggestBox);
    d->_filterMode = mode;
}

NAutoSuggestBox::FilterMode NAutoSuggestBox::filterMode() const {
    Q_D(const NAutoSuggestBox);
    return d->_filterMode;
}

QString NAutoSuggestBox::text() const {
    Q_D(const NAutoSuggestBox);
    return d->_lineEdit->text();
}

void NAutoSuggestBox::setText(const QString& text) {
    Q_D(NAutoSuggestBox);
    d->_lineEdit->setText(text);
}

void NAutoSuggestBox::clear() {
    Q_D(NAutoSuggestBox);
    d->_lineEdit->clear();
}

void NAutoSuggestBox::setPlaceholderText(const QString& placeholderText) {
    Q_D(NAutoSuggestBox);
    d->_lineEdit->setPlaceholderText(placeholderText);
}

QString NAutoSuggestBox::placeholderText() const {
    Q_D(const NAutoSuggestBox);
    return d->_lineEdit->placeholderText();
}

QString NAutoSuggestBox::addSuggestion(const QString& text, const QVariantMap& data) {
    Q_D(NAutoSuggestBox);
    NAutoSuggestion* suggestion = new NAutoSuggestion(this);
    suggestion->setText(text);
    suggestion->setData(data);
    d->_suggestions.append(suggestion);
    return suggestion->getKey();
}

QString NAutoSuggestBox::addSuggestion(NRegularIconType::Icon icon, const QString& text, const QVariantMap& data) {
    Q_D(NAutoSuggestBox);
    NAutoSuggestion* suggestion = new NAutoSuggestion(this);
    suggestion->setIcon(icon);
    suggestion->setText(text);
    suggestion->setData(data);
    d->_suggestions.append(suggestion);
    return suggestion->getKey();
}

void NAutoSuggestBox::removeSuggestion(const QString& key) {
    Q_D(NAutoSuggestBox);
    for (int i = 0; i < d->_suggestions.size(); ++i) {
        if (d->_suggestions[i]->getKey() == key) {
            NAutoSuggestion* suggestion = d->_suggestions.takeAt(i);
            suggestion->deleteLater();
            break;
        }
    }
    d->onTextChanged(d->_lineEdit->text());
}

void NAutoSuggestBox::removeSuggestion(int index) {
    Q_D(NAutoSuggestBox);
    if (index >= 0 && index < d->_suggestions.size()) {
        NAutoSuggestion* suggestion = d->_suggestions.takeAt(index);
        suggestion->deleteLater();

        // 更新过滤后的列表
        d->onTextChanged(d->_lineEdit->text());
    }
}

void NAutoSuggestBox::clearSuggestions() {
    Q_D(NAutoSuggestBox);
    qDeleteAll(d->_suggestions);
    d->_suggestions.clear();
    d->_model->clearSuggestions();
}