#ifndef QTNATIVEUI_NENUMS_H
#define QTNATIVEUI_NENUMS_H
#include <QObject>
#include "stdafx.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#define Q_BEGIN_ENUM_CREATE(CLASS)                                                                                     \
    namespace CLASS {                                                                                                  \
    Q_NAMESPACE_EXPORT(QTNATIVEUI_EXPORT)

#define Q_END_ENUM_CREATE(CLASS) }

#define Q_ENUM_CREATE(CLASS) Q_ENUM_NS(CLASS)
#else
#define Q_BEGIN_ENUM_CREATE(CLASS)                                                                                     \
    class QTNATIVEUI_EXPORT CLASS : public QObject {                                                                   \
        Q_OBJECT                                                                                                       \
      public:

#define Q_END_ENUM_CREATE(CLASS)                                                                                       \
  private:                                                                                                             \
    Q_DISABLE_COPY(CLASS)                                                                                              \
    }                                                                                                                  \
    ;

#define Q_ENUM_CREATE(CLASS) Q_ENUM(CLASS)
#endif

// 主题模式枚举
Q_BEGIN_ENUM_CREATE(NThemeType)

enum ThemeMode {
    Light, // 亮色主题
    Dark,  // 暗色主题
    System // 跟随系统主题
};
Q_ENUM_CREATE(ThemeMode)

Q_END_ENUM_CREATE(NThemeType)

#endif // QTNATIVEUI_NENUMS_H