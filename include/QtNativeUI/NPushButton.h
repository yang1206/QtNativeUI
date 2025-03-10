#ifndef QTNATIVEUI_NPUSHBUTTON_H
#define QTNATIVEUI_NPUSHBUTTON_H

#include <QtNativeUI/QtNativeUI_export.h>
#include <QtNativeUI/NMacros.h>
#include <QtNativeUI/NTheme.h>
#include <QPushButton>

namespace QtNativeUI {

/**
 * @brief Fluent Design 风格的按钮控件
 * 
 * 提供符合 Fluent Design 视觉语言的按钮，支持亮/暗色主题，
 */
class QTNATIVEUI_EXPORT NPushButton : public QPushButton
{
    Q_OBJECT
    
public:
    /**
     * @brief 按钮样式枚举
     */
    enum class NPushButtonStyle {
        Primary,    // 主要按钮，使用强调色
        Secondary,  // 次要按钮，使用中性颜色
        Text,       // 文本按钮，无背景色
        Outline,    // 轮廓按钮，有边框无背景
        Danger      // 危险操作按钮，使用错误色
    };
    Q_ENUM(NPushButtonStyle)
    
    // 使用宏创建属性
    N_PROPERTY(qreal, cornerRadius)
    N_PROPERTY(bool, elevationEnabled)
    N_PROPERTY(QColor, backgroundColor)
    N_PROPERTY(QColor, textColor)
    N_PROPERTY(QColor, borderColor)
    
    // buttonStyle 属性
    Q_PROPERTY(NPushButtonStyle buttonStyle READ buttonStyle WRITE setButtonStyle NOTIFY buttonStyleChanged)
public:
    Q_SIGNAL void buttonStyleChanged();
    void setButtonStyle(NPushButtonStyle value);
    NPushButtonStyle buttonStyle() const;
    
    explicit NPushButton(QWidget* parent = nullptr);
    explicit NPushButton(const QString& text, QWidget* parent = nullptr);
    ~NPushButton();
    
    // 重置外观到默认主题状态
    void resetAppearance();
    
    // 根据主题更新外观
    void updateFromTheme();
    
    // 返回推荐的尺寸提示
    QSize sizeHint() const override;
    
protected:
    // 重写绘制函数
    void paintEvent(QPaintEvent* event) override;
    
    // 重写鼠标事件处理
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    
    // 重写状态变化处理
    void changeEvent(QEvent* event) override;
    
private:
    class NPushButtonPrivate;
    N_DECLARE_PRIVATE(NPushButton)
};

} // namespace QtNativeUI

#endif // QTNATIVEUI_NPUSHBUTTON_H 