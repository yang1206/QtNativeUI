#include "IconsPage.h"
#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QLabel>
#include <QMetaEnum>
#include <QVBoxLayout>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NToolTip.h>

IconButton::IconButton(quint32 iconCode, const QString& name, bool isFilled, QWidget* parent)
    : NPushButton(parent), m_iconName(name), m_iconCode(iconCode), m_isFilled(isFilled) {
    setFixedSize(64, 64);
    setToolTip(QString("%1 (0x%2)").arg(name).arg(iconCode, 0, 16));
    new NToolTipFilter(this, 300, NToolTipPosition::TOP);

    QIcon icon;
    if (isFilled) {
        icon = nIcon->fromFilled(static_cast<NFilledIconType::Icon>(iconCode), 32);
    } else {
        icon = nIcon->fromRegular(static_cast<NRegularIconType::Icon>(iconCode), 32);
    }
    setIcon(icon);
    setIconSize(QSize(32, 32));
}

IconsPage::IconsPage(QWidget* parent)
    : BasePage("Icons 图标", "展示 QtNativeUI 的 Fluent 图标系统，包括 Regular 和 Filled 两种风格。", parent) {

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(16);

    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel*      searchLabel  = new QLabel("搜索:");
    m_searchEdit              = new NLineEdit();
    m_searchEdit->setPlaceholderText("输入图标名称搜索");
    m_searchEdit->setClearButtonEnabled(true);
    auto searchAction =
        m_searchEdit->addAction(NFilledIconType::Search16Filled, NLineEdit::ActionPosition::TrailingPosition);

    m_searchTimer = new QTimer(this);
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(300);

    connect(m_searchEdit, &QLineEdit::textChanged, this, &IconsPage::onSearchTextChanged);
    connect(searchAction, &QAction::triggered, this, &IconsPage::onSearchTextChanged);
    connect(m_searchTimer, &QTimer::timeout, this, &IconsPage::applyFilter);

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_searchEdit);

    mainLayout->addLayout(searchLayout);

    QHBoxLayout* typeFilterLayout = new QHBoxLayout();
    QLabel*      typeLabel        = new QLabel("图标类型:");

    m_iconTypeGroup = new QButtonGroup(this);
    m_allTypesRadio = new NRadioButton("全部");
    m_regularRadio  = new NRadioButton("Regular");
    m_filledRadio   = new NRadioButton("Filled");

    m_iconTypeGroup->addButton(m_allTypesRadio, IconFilterType::All);
    m_iconTypeGroup->addButton(m_regularRadio, IconFilterType::RegularOnly);
    m_iconTypeGroup->addButton(m_filledRadio, IconFilterType::FilledOnly);
    m_allTypesRadio->setChecked(true);

    connect(m_iconTypeGroup, &QButtonGroup::idClicked, this, &IconsPage::onIconTypeChanged);

    typeFilterLayout->addWidget(typeLabel);
    typeFilterLayout->addWidget(m_allTypesRadio);
    typeFilterLayout->addWidget(m_regularRadio);
    typeFilterLayout->addWidget(m_filledRadio);
    typeFilterLayout->addStretch();

    mainLayout->addLayout(typeFilterLayout);

    m_contentWidget = new QWidget();
    m_gridLayout    = new QGridLayout(m_contentWidget);
    m_gridLayout->setSpacing(8);
    m_gridLayout->setContentsMargins(4, 4, 4, 4);

    for (int i = 0; i < GRID_COLUMNS; ++i) {
        m_gridLayout->setColumnStretch(i, 1);
    }

    mainLayout->addWidget(m_contentWidget);

    QHBoxLayout* pageControlLayout = new QHBoxLayout();
    m_prevButton                   = new NPushButton("上一页");
    m_nextButton                   = new NPushButton("下一页");
    m_pageLabel                    = new QLabel("页 1/1");

    connect(m_prevButton, &NPushButton::clicked, this, [this]() {
        if (m_currentPage > 0) {
            m_currentPage--;
            loadIconsForPage(m_currentPage);
        }
    });

    connect(m_nextButton, &NPushButton::clicked, this, [this]() {
        int totalPages = (m_filteredIcons.size() + ICONS_PER_PAGE - 1) / ICONS_PER_PAGE;
        if (m_currentPage < totalPages - 1) {
            m_currentPage++;
            loadIconsForPage(m_currentPage);
        }
    });

    pageControlLayout->addStretch();
    pageControlLayout->addWidget(m_prevButton);
    pageControlLayout->addWidget(m_pageLabel);
    pageControlLayout->addWidget(m_nextButton);
    pageControlLayout->addStretch();

    mainLayout->addLayout(pageControlLayout);

    m_statusLabel = new QLabel("正在加载图标...");
    mainLayout->addWidget(m_statusLabel);

    addSection("图标浏览器", mainLayout);

    loadIcons();
}

void IconsPage::onIconTypeChanged(int id) {
    m_iconFilterType = static_cast<IconFilterType>(id);
    applyFilter();
}

void IconsPage::loadIcons() {
    m_allIcons.clear();

    const QMetaEnum regularEnum = QMetaEnum::fromType<NRegularIconType::Icon>();
    if (!regularEnum.isValid()) {
        qWarning() << "无法获取Regular图标元对象信息!";
        return;
    }
    for (int i = 0; i < regularEnum.keyCount(); ++i) {
        const char* key   = regularEnum.key(i);
        int         value = regularEnum.value(i);

        if (value <= 0 || value > 0xffff)
            continue;

        IconInfo info;
        info.name     = QString(key);
        info.code     = static_cast<quint32>(value);
        info.isFilled = false;
        m_allIcons.append(info);
    }

    const QMetaEnum filledEnum = QMetaEnum::fromType<NFilledIconType::Icon>();
    if (!filledEnum.isValid()) {
        qWarning() << "无法获取Filled图标元对象信息!";
        return;
    }
    for (int i = 0; i < filledEnum.keyCount(); ++i) {
        const char* key   = filledEnum.key(i);
        int         value = filledEnum.value(i);

        if (value <= 0 || value > 0xffff)
            continue;

        IconInfo info;
        info.name     = QString(key);
        info.code     = static_cast<quint32>(value);
        info.isFilled = true;
        m_allIcons.append(info);
    }

    m_filteredIcons = m_allIcons;
    m_currentPage   = 0;

    loadIconsForPage(0);

    m_statusLabel->setText(QString("共 %1 个图标").arg(m_allIcons.size()));
}

void IconsPage::loadIconsForPage(int page) {
    for (auto button : m_currentButtons.values()) {
        m_gridLayout->removeWidget(button);
        delete button;
    }
    m_currentButtons.clear();

    int totalIcons = m_filteredIcons.size();
    int totalPages = (totalIcons + ICONS_PER_PAGE - 1) / ICONS_PER_PAGE;

    if (page >= totalPages) {
        page = totalPages > 0 ? totalPages - 1 : 0;
    }
    m_currentPage = page;

    m_pageLabel->setText(QString("页 %1/%2").arg(page + 1).arg(totalPages));
    m_prevButton->setEnabled(page > 0);
    m_nextButton->setEnabled(page < totalPages - 1);

    int startIndex = page * ICONS_PER_PAGE;
    int endIndex   = qMin(startIndex + ICONS_PER_PAGE, totalIcons);

    int row = 0, col = 0;
    for (int i = startIndex; i < endIndex; i++) {
        const IconInfo& info = m_filteredIcons[i];

        IconButton* button = new IconButton(info.code, info.name, info.isFilled, m_contentWidget);
        connect(button, &NPushButton::clicked, this, &IconsPage::onIconClicked);

        m_gridLayout->addWidget(button, row, col);
        m_currentButtons[i - startIndex] = button;

        col++;
        if (col >= GRID_COLUMNS) {
            col = 0;
            row++;
        }
    }

    m_statusLabel->setText(QString("显示 %1-%2/%3 个图标").arg(startIndex + 1).arg(endIndex).arg(totalIcons));
}

void IconsPage::onSearchTextChanged() {
    m_searchTimer->stop();
    m_searchTimer->start();
}

void IconsPage::applyFilter() {
    m_searchText = m_searchEdit->text().trimmed();
    m_filteredIcons.clear();

    for (const IconInfo& info : m_allIcons) {
        bool typeMatch = true;
        switch (m_iconFilterType) {
            case RegularOnly:
                typeMatch = !info.isFilled;
                break;
            case FilledOnly:
                typeMatch = info.isFilled;
                break;
            case All:
            default:
                typeMatch = true;
                break;
        }

        bool textMatch = m_searchText.isEmpty() || info.name.contains(m_searchText, Qt::CaseInsensitive);

        if (typeMatch && textMatch) {
            m_filteredIcons.append(info);
        }
    }

    m_currentPage = 0;
    loadIconsForPage(0);
}

void IconsPage::onIconClicked() {
    IconButton* button = qobject_cast<IconButton*>(sender());
    if (button) {
        copyIconCode(button);
    }
}

void IconsPage::copyIconCode(IconButton* button) {
    QString enumName;

    if (button->isFilled()) {
        enumName = QString("NFilledIconType::%1").arg(button->iconName());
    } else {
        enumName = QString("NRegularIconType::%1").arg(button->iconName());
    }

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(enumName);
    NToolTip::showText(QString("已复制: %1").arg(enumName), button, NToolTipPosition::TOP);
}