#ifndef QTNATIVEUI_NPROGRESSBAR_P_H
#define QTNATIVEUI_NPROGRESSBAR_P_H

#include <QtNativeUI/NProgressBar.h>
#include "QtNativeUI/NEnums.h"

class NProgressBarPrivate : public QObject {
    Q_OBJECT

public:
    explicit NProgressBarPrivate(QObject* parent = nullptr);
    ~NProgressBarPrivate();
    Q_D_CREATE(NProgressBar)

    NThemeType::ThemeMode _themeMode;
    bool _isDark{false};
};

#endif // QTNATIVEUI_NPROGRESSBAR_P_H