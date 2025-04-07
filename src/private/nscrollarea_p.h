#ifndef QTNATIVEUI_NSCROLLAREA_P_H
#define QTNATIVEUI_NSCROLLAREA_P_H

#include <QtNativeUI/NScrollArea.h>
#include "QtNativeUI/NEnums.h"

class NScrollAreaPrivate : public QObject {
    Q_OBJECT

public:
    explicit NScrollAreaPrivate(QObject* parent = nullptr);
    ~NScrollAreaPrivate();
    Q_D_CREATE(NScrollArea)

    NThemeType::ThemeMode _themeMode;
    bool _isDark{false};
};

#endif // QTNATIVEUI_NSCROLLAREA_P_H