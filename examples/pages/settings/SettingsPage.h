#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "../BasePage.h"

class NComboBox;
class NRadioButton;
class NPushButton;
class QButtonGroup;

class SettingsPage : public BasePage {
    Q_OBJECT

  public:
    explicit SettingsPage(QWidget* parent = nullptr);

  private slots:
    void onThemeModeChanged(int id);
    void onAccentColorChanged();
    void onLanguageChanged(int index);
    void onUseSystemAccentColor();
    void onCustomAccentColor();
    void onResetToDefaults();

  private:
    void updateAccentColorButtons();

    QButtonGroup* m_themeModeGroup;
    NRadioButton* m_lightModeRadio;
    NRadioButton* m_darkModeRadio;
    NRadioButton* m_systemModeRadio;

    QList<NPushButton*> m_accentColorButtons;
    NPushButton*        m_customColorButton;
    NPushButton*        m_systemColorButton;

    NComboBox* m_languageCombo;
};

#endif // SETTINGSPAGE_H
