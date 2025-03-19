#ifndef QTNATIVEUI_NMENU_H
#define QTNATIVEUI_NMENU_H

#include <QMenu>
#include "NIconEnums.h"
#include "stdafx.h"

class NMenuPrivate;
class QTNATIVEUI_EXPORT NMenu : public QMenu {
    Q_OBJECT
    Q_Q_CREATE(NMenu)

  public:
    explicit NMenu(QWidget* parent = nullptr);
    explicit NMenu(const QString& title, QWidget* parent = nullptr);
    ~NMenu();

    // 添加菜单项
    QAction* addItem(const QString& text);
    QAction* addItem(const QString& text, NRegularIconType::Icon icon);
    QAction* addItem(const QString& text, NFilledIconType::Icon icon);
    
    // 添加子菜单
    NMenu* addSubMenu(const QString& text);
    NMenu* addSubMenu(const QString& text, NRegularIconType::Icon icon);
    NMenu* addSubMenu(const QString& text, NFilledIconType::Icon icon);

    // 添加分隔线
    void addSeparator();

  private:
    void init();
    QAction* createAction(const QString& text, const QIcon& icon = QIcon());
};

#endif // QTNATIVEUI_NMENU_H