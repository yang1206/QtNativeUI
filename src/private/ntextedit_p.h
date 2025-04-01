//
// Created by Yang1206 on 2025/4/1.
//

#ifndef NTEXTEDIT_P_H
#define NTEXTEDIT_P_H
#include <QtNativeUI/NTextEdit.h>

class NTextEditPrivate : public QObject {
    Q_OBJECT
  public:
    explicit NTextEditPrivate(QObject* parent = nullptr);
    ~NTextEditPrivate();
    Q_D_CREATE(NTextEdit)
};

#endif // NTEXTEDIT_P_H