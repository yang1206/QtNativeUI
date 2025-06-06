#ifndef QTNATIVEUI_NLABEL_H
#define QTNATIVEUI_NLABEL_H

#include <QLabel>
#include "NEnums.h"
#include "stdafx.h"

class NLabelPrivate;

/**
 * @brief 基于Fluent Design的标签控件
 *
 * NLabel是一个封装了QLabel的控件，支持Fluent Design的排版系统，
 * 可以轻松设置不同类型的文本样式（标题、正文、说明等），
 * 自动根据当前主题适配颜色。
 */
class QTNATIVEUI_EXPORT NLabel : public QLabel {
    Q_OBJECT
    Q_Q_CREATE(NLabel)

    // 标签类型
    Q_PROPERTY(NLabelType::Type type READ type WRITE setType NOTIFY typeChanged)
    // 文本颜色
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextDisabledColor)

  public:
    /**
     * @brief 构造一个NLabel
     * @param parent 父widget
     */
    explicit NLabel(QWidget* parent = nullptr);

    /**
     * @brief 构造一个具有指定文本的NLabel
     * @param text 显示的文本
     * @param parent 父widget
     */
    explicit NLabel(const QString& text, QWidget* parent = nullptr);

    /**
     * @brief 构造一个具有指定文本和类型的NLabel
     * @param text 显示的文本
     * @param type 标签类型
     * @param parent 父widget
     */
    explicit NLabel(const QString& text, NLabelType::Type type, QWidget* parent = nullptr);

    ~NLabel() override;

    /**
     * @brief 获取当前标签类型
     * @return 标签类型
     */
    NLabelType::Type type() const;

    /**
     * @brief 设置标签类型
     * @param type 标签类型
     */
    void setType(NLabelType::Type type);

  signals:
    /**
     * @brief 标签类型改变信号
     */
    void typeChanged();

  private:
    /**
     * @brief 初始化控件
     */
    void init();

    /**
     * @brief 更新控件样式
     */
    void updateStyle();
};

#endif // QTNATIVEUI_NLABEL_H