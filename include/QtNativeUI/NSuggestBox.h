#ifndef NSUGGESTBOX_H
#define NSUGGESTBOX_H

#include <QVariantMap>
#include <QWidget>

#include "NIconEnums.h"
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NSuggestBoxPrivate;
class NSuggestion;

class QTNATIVEUI_EXPORT NSuggestBox : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NSuggestBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(Qt::CaseSensitivity, CaseSensitivity)

  public:
    explicit NSuggestBox(QWidget* parent = nullptr);
    ~NSuggestBox();

    // 设置提示文本
    void    setPlaceholderText(const QString& placeholderText);
    QString placeholderText() const;

    // 添加建议项
    QString addSuggestion(const QString& text, const QVariantMap& data = {});
    QString addSuggestion(NRegularIconType::Icon icon, const QString& text, const QVariantMap& data = {});

    // 移除建议项
    void removeSuggestion(const QString& key);
    void removeSuggestion(int index);

    // 清空所有建议项
    void clearSuggestions();

  signals:
    void suggestionClicked(QString text, QVariantMap data);
};

#endif // NSUGGESTBOX_H