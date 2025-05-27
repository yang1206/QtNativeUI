#ifndef QTNATIVEUI_NMAINWINDOW_P_H
#define QTNATIVEUI_NMAINWINDOW_P_H

#include "QtNativeUI/NMainWindow.h"

class NMainWindowPrivate : public QObject {
    Q_OBJECT
  public:
    explicit NMainWindowPrivate(QObject* parent = nullptr);
    ~NMainWindowPrivate() override;

    NMainWindow* q_ptr;
    // 背景效果类型
    NMainWindow::BackdropType backdropEffect = NMainWindow::None;
    // 主题相关
    bool isDarkMode = false;
    // 窗口状态
    bool isWindowActive = true;
    // 窗口属性
    QColor backgroundColor;

    // 设置背景效果
    void setBackdropEffect(NMainWindow::BackdropType type);

    // 更新窗口背景颜色（用于None模式）
    void updateBackgroundColor();
};

#endif // QTNATIVEUI_NMAINWINDOW_P_H