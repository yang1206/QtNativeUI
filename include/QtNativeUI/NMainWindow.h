#ifndef QTNATIVEUI_NMAINWINDOW_H
#define QTNATIVEUI_NMAINWINDOW_H

#include <QMainWindow>
#include "NEnums.h"
#include "stdafx.h"

class NMainWindowPrivate;
class QTNATIVEUI_EXPORT NMainWindow : public QMainWindow {
    Q_OBJECT
    Q_Q_CREATE(NMainWindow)
  public:
    enum BackdropType {
        None    = 1, // 无特殊效果
        Mica    = 2, // Mica效果 (Windows 11)
        Acrylic = 3, // 亚克力效果 (Windows 10/11)
        Tabbed  = 4  // 标签式效果 (Windows 11)
    };
    Q_ENUM(BackdropType)

    explicit NMainWindow(QWidget* parent = nullptr);
    ~NMainWindow();

    bool applyBackdropEffect(BackdropType type);

    void setBackdropEffect(BackdropType type);
    int  getBackdropEffect();

    BackdropType backdropEffect() const;

    // 启用/禁用窗口动画
    void enableWindowAnimation(bool enable);

  protected:
    virtual void initWindow();
    bool         eventFilter(QObject* watched, QEvent* event) override;

  private:
    // 初始化平台特定窗口效果
    void initPlatformEffect();

    // 平台特定效果设置
    bool setPlatformEffect(BackdropType type);

    BackdropType getDefaultEffect();

    // 保护子控件样式的方法
    void protectWidgetStyle(QWidget* widget);
    void updateChildrenStyles(QWidget* parent);
};

#endif // QTNATIVEUI_NMAINWINDOW_H
