#ifndef QTNATIVEUI_NMAINWINDOW_H
#define QTNATIVEUI_NMAINWINDOW_H

#include <QMainWindow>
#include "stdafx.h"

class NMainWindowPrivate;
class QTNATIVEUI_EXPORT NMainWindow : public QMainWindow {
    Q_OBJECT
    Q_PROPERTY(BackdropType backdropEffect READ backdropEffect WRITE setBackdropEffect NOTIFY backdropEffectChanged)
    Q_PROPERTY(bool isDarkMode READ isDarkMode NOTIFY darkModeChanged)
  public:
    enum BackdropType {
        None    = 1, // 无特殊效果
        Mica    = 2, // Mica效果 (Windows 11)
        Acrylic = 3, // 亚克力效果 (Windows 10/11)
        MicaAlt = 4, // MicaAlt效果 (Windows 11)
        DWMBlur = 5  // DWM模糊效果 (Windows Vista+)
    };
    Q_ENUM(BackdropType)

    explicit NMainWindow(QWidget* parent = nullptr);
    ~NMainWindow() override;

    // 窗口效果相关
    bool         applyBackdropEffect(BackdropType type);
    void         setBackdropEffect(BackdropType type);
    BackdropType backdropEffect() const;
    // 主题相关
    bool isDarkMode() const;

    // 窗口风格相关
    void enableWindowAnimation(bool enable);
    void setRoundedCorners(bool enable);
    // 窗口阴影设置
    void setShadowEffect(bool enable);

  protected:
    // 事件处理
    bool eventFilter(QObject* watched, QEvent* event) override;
    void changeEvent(QEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
    void paintEvent(QPaintEvent* event) override;

    // 初始化相关
    virtual void initWindow();

  signals:
    void backdropEffectChanged(BackdropType type);
    void darkModeChanged(bool isDark);

  private:
    void         initPlatformEffect();
    bool         setPlatformEffect(BackdropType type);
    BackdropType getDefaultEffect();

    // 私有实现
    NMainWindowPrivate* d_ptr;
    Q_DECLARE_PRIVATE(NMainWindow)
};

#endif // QTNATIVEUI_NMAINWINDOW_H