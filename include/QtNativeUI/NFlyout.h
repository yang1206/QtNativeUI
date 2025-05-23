#ifndef QTNATIVEUI_NFLYOUT_H
#define QTNATIVEUI_NFLYOUT_H

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QWidget>
#include "NColor.h"
#include "stdafx.h"

class NFlyoutViewBase;
class NFlyoutPrivate;
class NFlyoutAnimationManager;

/**
 * @brief 定义Flyout动画类型
 */
enum class NFlyoutAnimationType {
    PULL_UP,     // 向上弹出
    DROP_DOWN,   // 向下弹出
    SLIDE_LEFT,  // 从右向左滑入
    SLIDE_RIGHT, // 从左向右滑入
    FADE_IN,     // 淡入
    NONE         // 无动画
};

/**
 * @brief Flyout组件
 */
class QTNATIVEUI_EXPORT NFlyout : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NFlyout)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BorderWidth)
    Q_PROPERTY_CREATE_Q_H(Qt::Edge, Placement)

  public:
    explicit NFlyout(QWidget* parent = nullptr);
    explicit NFlyout(QWidget* content, QWidget* parent = nullptr);
    ~NFlyout() override;

    /**
     * @brief 设置Flyout内容
     */
    void setContent(QWidget* content);

    /**
     * @brief 获取Flyout内容
     */
    QWidget* content() const;

    /**
     * @brief 设置目标控件
     */
    void setTarget(QWidget* target);

    /**
     * @brief 获取目标控件
     */
    QWidget* target() const;

    /**
     * @brief 判断Flyout是否已打开
     */
    bool isOpen() const;

    /**
     * @brief 设置动画类型
     */
    void setAnimationType(NFlyoutAnimationType type);

    /**
     * @brief 获取动画类型
     */
    NFlyoutAnimationType animationType() const;

    /**
     * @brief 外部点击关闭模式
     */
    enum LightDismissMode {
        Auto, // 默认按平台决定
        On,   // 强制启用
        Off   // 强制禁用
    };
    Q_ENUM(LightDismissMode)

    /**
     * @brief 设置外部点击关闭模式
     */
    void setLightDismissMode(LightDismissMode mode);

    /**
     * @brief 获取外部点击关闭模式
     */
    LightDismissMode lightDismissMode() const;

    /**
     * @brief 设置阴影效果
     */
    void setShadowEffect(int blurRadius = 35, const QPoint& offset = QPoint(0, 8));

    /**
     * @brief 创建并显示Flyout
     * @param content 内容组件
     * @param target 目标组件或位置
     * @param parent 父组件
     * @param animType 动画类型
     * @param isDeleteOnClose 关闭时是否自动删除
     * @return 创建的Flyout实例
     */
    static NFlyout* make(QWidget*             content,
                         QWidget*             target,
                         QWidget*             parent          = nullptr,
                         NFlyoutAnimationType animType        = NFlyoutAnimationType::PULL_UP,
                         bool                 isDeleteOnClose = true);

  public slots:
    /**
     * @brief 在指定位置显示Flyout
     * @param target 目标控件
     */
    void showAt(QWidget* target);

    /**
     * @brief 在指定坐标显示Flyout
     * @param pos 全局坐标
     */
    void exec(const QPoint& pos);

    /**
     * @brief 隐藏Flyout
     */
    void hide();

    /**
     * @brief 淡出并关闭
     */
    void fadeOut();

  signals:
    void opened();
    void opening();
    void closed();
    void closing();

  protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

  private:
    void   updatePosition();
    void   setupEventFilter();
    QPoint calculatePositionForTarget(QWidget* target);

    friend class NFlyoutAnimationManager;
};

#endif // QTNATIVEUI_NFLYOUT_H