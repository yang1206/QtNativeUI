#ifndef QTNATIVEUI_NMAINWINDOW_H
#define QTNATIVEUI_NMAINWINDOW_H

#include <QMainWindow>
#include "NEnums.h"
#include "stdafx.h"

class NMainWindowPrivate;
class QTNATIVEUI_EXPORT NMainWindow : public QMainWindow {
    Q_OBJECT
    Q_Q_CREATE(NMainWindow)
    Q_PROPERTY_CREATE_Q_H(int, BackdropEffect)

public:
    enum BackdropType {
        Default = 0,    // 自动选择最佳效果（平台相关）
        None = 1,       // 无特殊效果
        Mica = 2,       // Mica效果 (Windows 11)
        Acrylic = 3,    // 亚克力效果 (Windows 10/11)
        Tabbed = 4      // 标签式效果 (Windows 11)
    };
    Q_ENUM(BackdropType)

    explicit NMainWindow(QWidget* parent = nullptr);
    ~NMainWindow();

    // 设置窗口背景效果（使用枚举类型）
    bool applyBackdropEffect(BackdropType type);
    
    // 获取当前背景效果（枚举类型）
    BackdropType backdropEffect() const;

    // 启用/禁用窗口动画
    void enableWindowAnimation(bool enable);

protected:
    // 初始化窗口，子类可以重写此方法添加额外初始化
    virtual void initWindow();

private:
    // 初始化平台特定窗口效果
    void initPlatformEffect();
    
    // 平台特定效果设置
    bool setPlatformEffect(BackdropType type);

    BackdropType getDefaultEffect();
};

#endif // QTNATIVEUI_NMAINWINDOW_H
