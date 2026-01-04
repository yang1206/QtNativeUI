#ifndef QTNATIVEUI_NMENU_H
#define QTNATIVEUI_NMENU_H

#include <QMenu>
#include "NIconEnums.h"
#include "stdafx.h"

class NMenuPrivate;
class QTNATIVEUI_EXPORT NMenu : public QMenu {
    Q_OBJECT
    Q_Q_CREATE(NMenu)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightSeparatorColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkSeparatorColor)

  public:
    explicit NMenu(QWidget* parent = nullptr);
    explicit NMenu(const QString& title, QWidget* parent = nullptr);
    ~NMenu();

    void setDropDownAnimation(bool enabled);
    bool dropDownAnimation() const;

    // 添加菜单项
    QAction* addItem(const QString& text);
    QAction* addItem(const QString& text, NRegularIconType::Icon icon);
    QAction* addItem(const QString& text, NFilledIconType::Icon icon);

    QAction* addItem(const QString& text, const QKeySequence& shortcut);
    QAction* addItem(const QString& text, NRegularIconType::Icon icon, const QKeySequence& shortcut);
    QAction* addItem(const QString& text, NFilledIconType::Icon icon, const QKeySequence& shortcut);

    QAction* addCheckableItem(const QString& text, bool checked = false);
    QAction* addCheckableItem(const QString& text, NRegularIconType::Icon icon, bool checked = false);
    QAction* addCheckableItem(const QString& text, NFilledIconType::Icon icon, bool checked = false);

    // 添加子菜单
    NMenu* addSubMenu(const QString& text);
    NMenu* addSubMenu(const QString& text, NRegularIconType::Icon icon);
    NMenu* addSubMenu(const QString& text, NFilledIconType::Icon icon);

    void setItemEnabled(QAction* action, bool enabled);
    void setItemVisible(QAction* action, bool visible);

    // 设置菜单项工具提示
    void setItemToolTip(QAction* action, const QString& toolTip);

    // 添加分隔线
    void addSeparator();

  protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    bool event(QEvent* event) override;

  private:
    void     init();
    void     addMenuShadow();
    QAction* createAction(const QString& text, const QIcon& icon = QIcon());
};

#endif // QTNATIVEUI_NMENU_H