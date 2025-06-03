#ifndef QTNATIVEUI_NINFOBAR_H
#define QTNATIVEUI_NINFOBAR_H

#include <QWidget>
#include "NEnums.h"
#include "stdafx.h"

class NInfoBarPrivate;

class QTNATIVEUI_EXPORT NInfoBar : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NInfoBar)

  public:
    explicit NInfoBar(NInfoBarType::InfoMode severity,
                      const QString&         title,
                      const QString&         message,
                      QWidget*               parent          = nullptr,
                      bool                   showCloseButton = false);

    void        addWidget(QWidget* widget);
    static void information(NInfoBarType::PositionPolicy position,
                            QString                      title,
                            QString                      message,
                            int                          displayDuration = 3000,
                            QWidget*                     parent          = nullptr,
                            bool                         showCloseButton = true);
    static void success(NInfoBarType::PositionPolicy position,
                        QString                      title,
                        QString                      message,
                        int                          displayDuration = 3000,
                        QWidget*                     parent          = nullptr,
                        bool                         showCloseButton = true);
    static void warning(NInfoBarType::PositionPolicy position,
                        QString                      title,
                        QString                      message,
                        int                          displayDuration = 3000,
                        QWidget*                     parent          = nullptr,
                        bool                         showCloseButton = true);
    static void error(NInfoBarType::PositionPolicy position,
                      QString                      title,
                      QString                      message,
                      int                          displayDuration = 3000,
                      QWidget*                     parent          = nullptr,
                      bool                         showCloseButton = true);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

  private:
    friend class NInfoBarManager;
    explicit NInfoBar(NInfoBarType::PositionPolicy position,
                      NInfoBarType::InfoMode       severity,
                      QString&                     title,
                      QString&                     message,
                      int                          displayDuration,
                      QWidget*                     parent          = nullptr,
                      bool                         showCloseButton = true);
    ~NInfoBar();
};

#endif // QTNATIVEUI_NINFOBAR_H