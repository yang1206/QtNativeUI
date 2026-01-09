#ifndef QTNATIVEUI_NFLYOUT_P_H
#define QTNATIVEUI_NFLYOUT_P_H

#include <QGraphicsDropShadowEffect>
#include <QObject>

#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NFlyout.h>

class QHBoxLayout;
class QVBoxLayout;

class NFlyoutAnimationManager : public QObject {
    Q_OBJECT
  public:
    explicit NFlyoutAnimationManager(NFlyout* flyout, QObject* parent = nullptr);
    ~NFlyoutAnimationManager() override = default;

    virtual void   exec(const QPoint& pos);
    virtual QPoint position(QWidget* target);

    QPoint adjustPosition(const QPoint& pos) const;
    void   setup() const;

  protected:
    NFlyout*                 m_flyout;
    QParallelAnimationGroup* m_aniGroup;
    QPropertyAnimation*      m_slideAni;
    QPropertyAnimation*      m_opacityAni;

    static QHash<NFlyoutAnimationType, NFlyoutAnimationManager* (*) (NFlyout*)> s_managers;

  public:
    static NFlyoutAnimationManager* create(NFlyoutAnimationType type, NFlyout* flyout);

    template <typename T>
    static void registerManager(NFlyoutAnimationType type) {
        s_managers[type] = [](NFlyout* flyout) -> NFlyoutAnimationManager* { return new T(flyout); };
    }
};

class PullUpAnimationManager : public NFlyoutAnimationManager {
    Q_OBJECT
  public:
    PullUpAnimationManager(NFlyout* flyout) : NFlyoutAnimationManager(flyout) {}
    void   exec(const QPoint& pos) override;
    QPoint position(QWidget* target) override;
};

class DropDownAnimationManager : public NFlyoutAnimationManager {
    Q_OBJECT
  public:
    DropDownAnimationManager(NFlyout* flyout) : NFlyoutAnimationManager(flyout) {}
    void   exec(const QPoint& pos) override;
    QPoint position(QWidget* target) override;
};

class SlideLeftAnimationManager : public NFlyoutAnimationManager {
    Q_OBJECT
  public:
    SlideLeftAnimationManager(NFlyout* flyout) : NFlyoutAnimationManager(flyout) {}
    void   exec(const QPoint& pos) override;
    QPoint position(QWidget* target) override;
};

class SlideRightAnimationManager : public NFlyoutAnimationManager {
    Q_OBJECT
  public:
    SlideRightAnimationManager(NFlyout* flyout) : NFlyoutAnimationManager(flyout) {}
    void   exec(const QPoint& pos) override;
    QPoint position(QWidget* target) override;
};

class FadeInAnimationManager : public NFlyoutAnimationManager {
    Q_OBJECT
  public:
    FadeInAnimationManager(NFlyout* flyout) : NFlyoutAnimationManager(flyout) {}
    void   exec(const QPoint& pos) override;
    QPoint position(QWidget* target) override;
};

class NoneAnimationManager : public NFlyoutAnimationManager {
    Q_OBJECT
  public:
    NoneAnimationManager(NFlyout* flyout) : NFlyoutAnimationManager(flyout) {}
    void   exec(const QPoint& pos) override;
    QPoint position(QWidget* target) override;
};

class NFlyoutPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderWidth)
    Q_PROPERTY_CREATE_D(QWidget*, Content)
    Q_PROPERTY_CREATE_D(QWidget*, Target)
    Q_PROPERTY_CREATE_D(Qt::Edge, Placement)

  public:
    explicit NFlyoutPrivate(QObject* parent = nullptr);
    ~NFlyoutPrivate() override;
    Q_D_CREATE(NFlyout)

    void  setupUI();
    void  updateThemeColors();
    QRect calculatePlacement(const QWidget* target, Qt::Edge placement) const;

    NFlyoutAnimationManager* animationManager();

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    bool _isHovered;
    bool _isPressed;
    bool _isOpen;

    int                       _shadowBorderWidth;
    NFlyout::LightDismissMode _lightDismissMode;
    NFlyoutAnimationType      _animationType;
    bool                      _isDeleteOnClose;

    NFlyoutAnimationManager*   _animManager;
    QPropertyAnimation*        _fadeOutAnimation;
    QGraphicsDropShadowEffect* _shadowEffect;

    QHBoxLayout* _mainLayout;
};

#endif // QTNATIVEUI_NFLYOUT_P_H