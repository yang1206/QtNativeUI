#include "SettingsPage.h"
#include <QButtonGroup>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NComboBox.h>
#include <QtNativeUI/NConfig.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NRadioButton.h>
#include <QtNativeUI/NTheme.h>

SettingsPage::SettingsPage(QWidget* parent)
    : BasePage("Settings 设置", "配置应用程序的全局设置，包括主题、语言和强调色。", parent) {

    QVBoxLayout* themeModeLayout = new QVBoxLayout();
    themeModeLayout->setSpacing(12);

    m_themeModeGroup = new QButtonGroup(this);
    m_lightModeRadio = new NRadioButton("浅色模式");
    m_darkModeRadio  = new NRadioButton("深色模式");
    m_systemModeRadio = new NRadioButton("跟随系统");

    m_themeModeGroup->addButton(m_lightModeRadio, static_cast<int>(NThemeType::ThemeMode::Light));
    m_themeModeGroup->addButton(m_darkModeRadio, static_cast<int>(NThemeType::ThemeMode::Dark));
    m_themeModeGroup->addButton(m_systemModeRadio, static_cast<int>(NThemeType::ThemeMode::System));

    switch (nTheme->themeMode()) {
        case NThemeType::ThemeMode::Light:
            m_lightModeRadio->setChecked(true);
            break;
        case NThemeType::ThemeMode::Dark:
            m_darkModeRadio->setChecked(true);
            break;
        case NThemeType::ThemeMode::System:
            m_systemModeRadio->setChecked(true);
            break;
    }

    connect(m_themeModeGroup, &QButtonGroup::idClicked, this, &SettingsPage::onThemeModeChanged);

    themeModeLayout->addWidget(m_lightModeRadio);
    themeModeLayout->addWidget(m_darkModeRadio);
    themeModeLayout->addWidget(m_systemModeRadio);

    addSection("主题模式", themeModeLayout);

    QVBoxLayout* accentColorLayout = new QVBoxLayout();
    accentColorLayout->setSpacing(16);

    QLabel* accentLabel = new QLabel("选择强调色：");
    accentColorLayout->addWidget(accentLabel);

    QGridLayout* colorGrid = new QGridLayout();
    colorGrid->setSpacing(12);

    QList<QPair<QString, NAccentColorType::Type>> accentColors = {
        {"Yellow", NAccentColorType::Yellow},   {"Orange", NAccentColorType::Orange},
        {"Red", NAccentColorType::Red},         {"Magenta", NAccentColorType::Magenta},
        {"Purple", NAccentColorType::Purple},   {"Blue", NAccentColorType::Blue},
        {"Teal", NAccentColorType::Teal},       {"Green", NAccentColorType::Green}};

    int row = 0, col = 0;
    for (const auto& pair : accentColors) {
        NPushButton* btn = new NPushButton(pair.first);
        btn->setFixedSize(100, 40);
        btn->setAccentColor(nTheme->getAccentColor(pair.second).normal());
        btn->setProperty("accentType", static_cast<int>(pair.second));

        connect(btn, &NPushButton::clicked, this, &SettingsPage::onAccentColorChanged);

        m_accentColorButtons.append(btn);
        colorGrid->addWidget(btn, row, col);

        col++;
        if (col >= 4) {
            col = 0;
            row++;
        }
    }

    accentColorLayout->addLayout(colorGrid);

    QHBoxLayout* specialColorLayout = new QHBoxLayout();
    specialColorLayout->setSpacing(12);

    m_systemColorButton = new NPushButton("使用系统强调色");
    m_systemColorButton->setFixedSize(140, 40);
    connect(m_systemColorButton, &NPushButton::clicked, this, &SettingsPage::onUseSystemAccentColor);

    m_customColorButton = new NPushButton("自定义颜色");
    m_customColorButton->setFixedSize(140, 40);
    connect(m_customColorButton, &NPushButton::clicked, this, &SettingsPage::onCustomAccentColor);

    specialColorLayout->addWidget(m_systemColorButton);
    specialColorLayout->addWidget(m_customColorButton);
    specialColorLayout->addStretch();

    accentColorLayout->addLayout(specialColorLayout);

    addSection("强调色", accentColorLayout);

    QVBoxLayout* languageLayout = new QVBoxLayout();
    languageLayout->setSpacing(12);

    QLabel* languageLabel = new QLabel("选择语言：");
    m_languageCombo       = new NComboBox();
    m_languageCombo->setFixedWidth(200);

    QStringList languages = nConfig->availableLanguages();
    for (const QString& lang : languages) {
        if (lang == "en_US") {
            m_languageCombo->addItem("English", lang);
        } else if (lang == "zh_CN") {
            m_languageCombo->addItem("简体中文", lang);
        } else {
            m_languageCombo->addItem(lang, lang);
        }
    }

    QString currentLang = nConfig->currentLanguage();
    for (int i = 0; i < m_languageCombo->count(); ++i) {
        if (m_languageCombo->itemData(i).toString() == currentLang) {
            m_languageCombo->setCurrentIndex(i);
            break;
        }
    }

    connect(m_languageCombo, QOverload<int>::of(&NComboBox::currentIndexChanged), this,
            &SettingsPage::onLanguageChanged);

    languageLayout->addWidget(languageLabel);
    languageLayout->addWidget(m_languageCombo);

    addSection("语言", languageLayout);

    QVBoxLayout* resetLayout = new QVBoxLayout();
    resetLayout->setSpacing(12);

    QLabel* resetLabel = new QLabel("将所有设置恢复为默认值：");

    NPushButton* resetButton = new NPushButton("恢复默认设置");
    resetButton->setFixedSize(140, 40);
    connect(resetButton, &NPushButton::clicked, this, &SettingsPage::onResetToDefaults);

    resetLayout->addWidget(resetLabel);
    resetLayout->addWidget(resetButton);

    addSection("重置", resetLayout);

    updateAccentColorButtons();

    connect(nTheme, &NTheme::accentColorChanged, this, &SettingsPage::updateAccentColorButtons);
}

void SettingsPage::onThemeModeChanged(int id) {
    nTheme->setThemeMode(static_cast<NThemeType::ThemeMode>(id));
}

void SettingsPage::onAccentColorChanged() {
    NPushButton* btn = qobject_cast<NPushButton*>(sender());
    if (!btn)
        return;

    int                     type        = btn->property("accentType").toInt();
    NAccentColorType::Type  accentType  = static_cast<NAccentColorType::Type>(type);
    NAccentColor            accentColor = nTheme->getAccentColor(accentType);

    nTheme->setAccentColor(accentColor);
}

void SettingsPage::onLanguageChanged(int index) {
    QString locale = m_languageCombo->itemData(index).toString();
    if (nConfig->setLanguage(locale)) {
        QWidget* topLevel = window();
        if (topLevel) {
            QEvent event(QEvent::LanguageChange);
            QApplication::sendEvent(topLevel, &event);
            
            QList<QWidget*> allWidgets = topLevel->findChildren<QWidget*>();
            for (QWidget* widget : allWidgets) {
                QApplication::sendEvent(widget, &event);
            }
        }
    }
}

void SettingsPage::onUseSystemAccentColor() {
    nTheme->useSystemAccentColor();
    updateAccentColorButtons();
}

void SettingsPage::onCustomAccentColor() {
    QColor currentColor = nTheme->accentColor().normal();
    QColor color        = QColorDialog::getColor(currentColor, this, "选择自定义强调色");

    if (color.isValid()) {
        nTheme->setAccentColor(color);
    }
}

void SettingsPage::onResetToDefaults() {
    nTheme->resetToDefaults();
    nTheme->setThemeMode(NThemeType::ThemeMode::System);

    m_systemModeRadio->setChecked(true);
    updateAccentColorButtons();
}

void SettingsPage::updateAccentColorButtons() {
    QColor currentAccent = nTheme->accentColor().normal();

    for (NPushButton* btn : m_accentColorButtons) {
        int                    type       = btn->property("accentType").toInt();
        NAccentColorType::Type accentType = static_cast<NAccentColorType::Type>(type);
        QColor                 btnColor   = nTheme->getAccentColor(accentType).normal();

        bool isSelected = qAbs(currentAccent.red() - btnColor.red()) < 10 &&
                          qAbs(currentAccent.green() - btnColor.green()) < 10 &&
                          qAbs(currentAccent.blue() - btnColor.blue()) < 10;

        if (isSelected) {
            btn->setButtonType(NPushButton::Accent);
        } else {
            btn->setButtonType(NPushButton::Standard);
        }
    }
}
