#ifndef INFOBARPAGE_H
#define INFOBARPAGE_H

#include "../BasePage.h"

class NInfoBar;

class InfoBarPage : public BasePage {
    Q_OBJECT

  public:
    explicit InfoBarPage(QWidget* parent = nullptr);

  private:
    NInfoBar* _persistentInfoBar{nullptr};
};

#endif // INFOBARPAGE_H
