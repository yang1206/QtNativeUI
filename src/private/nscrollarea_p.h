#ifndef QTNATIVEUI_NSCROLLAREA_P_H
#define QTNATIVEUI_NSCROLLAREA_P_H

#include <QtNativeUI/NScrollArea.h>
#include <QtNativeUI/NScrollBar.h>
#include "QtNativeUI/NEnums.h"

class NScrollAreaPrivate : public QObject {
    Q_OBJECT

public:
    explicit NScrollAreaPrivate(QObject* parent = nullptr);
    ~NScrollAreaPrivate();
    Q_D_CREATE(NScrollArea)

    NThemeType::ThemeMode _themeMode;
    bool _isDark{false};
    bool _useCustomScrollBars{true};
    bool _hasTransparentBackground{false};
    
    NScrollBar* _horizontalScrollBar{nullptr};
    NScrollBar* _verticalScrollBar{nullptr};
    
    // 保存原始滚动条用于恢复
    QScrollBar* _originalHorizontalScrollBar{nullptr};
    QScrollBar* _originalVerticalScrollBar{nullptr};
    
    // 视口边距
    QMargins _viewportMargins{0, 0, 0, 0};
    
    // 内容区域
    QRect _contentRect;
    
    // 更新内容区域
    void updateContentRect();
};

#endif // QTNATIVEUI_NSCROLLAREA_P_H