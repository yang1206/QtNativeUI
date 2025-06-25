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
    enum FilterMode {
        Contains,   // 默认，包含搜索文本
        StartsWith, // 以搜索文本开头
        EndsWith,   // 以搜索文本结尾
        Equals      // 完全匹配
    };
    Q_ENUM(FilterMode)

    // 设置过滤模式
    void       setFilterMode(FilterMode mode);
    FilterMode filterMode() const;
    // 获取/设置当前输入文本
    QString text() const;
    void    setText(const QString& text);
    void    clear();
    // 设置提示文本
    void    setPlaceholderText(const QString& placeholderText);
    QString placeholderText() const;
    QString addSuggestion(const QString& text, const QVariantMap& data = {});
    QString addSuggestion(NRegularIconType::Icon icon, const QString& text, const QVariantMap& data = {});
    QString addSuggestion(NFilledIconType::Icon icon, const QString& text, const QVariantMap& data = {});
    void    removeSuggestion(const QString& key);
    void    removeSuggestion(int index);

    // 清空所有建议项
    void clearSuggestions();

    void setFocus();

  signals:
    void suggestionClicked(QString text, QVariantMap data);
    void querySubmitted(const QString& query);
};

#endif // NAUTOSUGGESTBOX_H