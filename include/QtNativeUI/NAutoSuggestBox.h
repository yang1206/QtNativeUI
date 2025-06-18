#ifndef NAUTOSUGGESTBOX_H
#define NAUTOSUGGESTBOX_H

#include <QVariantMap>
#include <QWidget>

#include "NIconEnums.h"
#include "QtNativeUI/stdafx.h"

class NAutoSuggestBoxPrivate;
class NAutoSuggestion;

class QTNATIVEUI_EXPORT NAutoSuggestBox : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NAutoSuggestBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(Qt::CaseSensitivity, CaseSensitivity)

  public:
    explicit NAutoSuggestBox(QWidget* parent = nullptr);
    ~NAutoSuggestBox();

    // 设置提示文本
    void    setPlaceholderText(const QString& placeholderText);
    QString placeholderText() const;
    QString addSuggestion(const QString& text, const QVariantMap& data = {});
    QString addSuggestion(NRegularIconType::Icon icon, const QString& text, const QVariantMap& data = {});
    void    removeSuggestion(const QString& key);
    void    removeSuggestion(int index);

    // 清空所有建议项
    void clearSuggestions();

  signals:
    void suggestionClicked(QString text, QVariantMap data);
};

#endif // NAUTOSUGGESTBOX_H