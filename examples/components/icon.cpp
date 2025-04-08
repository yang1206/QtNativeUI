#include "icon.h"
#include <QApplication>
#include <QClipboard>
#include <QMetaEnum>
#include <QRegularExpression>
#include <QToolTip>
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NScrollArea.h"

IconButton::IconButton(quint32 iconCode, const QString& name, bool isFilled, QWidget* parent)
    : NPushButton(parent), m_iconName(name), m_iconCode(iconCode), m_isFilled(isFilled) {
    // 使用固定尺寸
    setFixedSize(64, 64);
    setToolTip(QString("%1 (0x%2)").arg(name).arg(iconCode, 0, 16));

    // 设置图标
    QIcon icon;
    if (isFilled) {
        icon = nIcon->fromFilled(static_cast<NFilledIconType::Icon>(iconCode), 32);
    } else {
        icon = nIcon->fromRegular(static_cast<NRegularIconType::Icon>(iconCode), 32);
    }
    setIcon(icon);
    setIconSize(QSize(32, 32));
}

IconExample::IconExample(QWidget* parent) : QWidget(parent) {
    initUI();
    loadIcons();
}

void IconExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    // 减小布局边距，确保内容更紧凑
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // 顶部搜索栏
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel*      searchLabel  = new QLabel("搜索:", this);
    m_searchEdit              = new NLineEdit(this);
    m_searchEdit->setPlaceholderText("输入图标名称搜索");
    m_searchEdit->setClearButtonEnabled(true);
    auto searchAction =
        m_searchEdit->addAction(NFilledIconType::Search16Filled, NLineEdit::ActionPosition::TrailingPosition);

    // 使用计时器延迟搜索，减少输入时的卡顿
    m_searchTimer = new QTimer(this);
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(300); // 300ms延迟

    connect(m_searchEdit, &QLineEdit::textChanged, this, &IconExample::onSearchTextChanged);
    connect(searchAction, &QAction::triggered, this, &IconExample::onSearchTextChanged);
    connect(m_searchTimer, &QTimer::timeout, this, &IconExample::applyFilter);

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_searchEdit);

    // 添加图标类型筛选选项
    QHBoxLayout* typeFilterLayout = new QHBoxLayout();
    QLabel*      typeLabel        = new QLabel("图标类型:", this);

    m_iconTypeGroup = new QButtonGroup(this);
    m_allTypesRadio = new NRadioButton("全部", this);
    m_regularRadio  = new NRadioButton("Regular", this);
    m_filledRadio   = new NRadioButton("Filled", this);

    m_iconTypeGroup->addButton(m_allTypesRadio, IconFilterType::All);
    m_iconTypeGroup->addButton(m_regularRadio, IconFilterType::RegularOnly);
    m_iconTypeGroup->addButton(m_filledRadio, IconFilterType::FilledOnly);
    m_allTypesRadio->setChecked(true);

    connect(m_iconTypeGroup, &QButtonGroup::idClicked, this, &IconExample::onIconTypeChanged);

    typeFilterLayout->addWidget(typeLabel);
    typeFilterLayout->addWidget(m_allTypesRadio);
    typeFilterLayout->addWidget(m_regularRadio);
    typeFilterLayout->addWidget(m_filledRadio);
    typeFilterLayout->addStretch();

    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(typeFilterLayout);

    // 图标显示区域
    m_scrollArea = new NScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_contentWidget = new QWidget(m_scrollArea);
    m_gridLayout    = new QGridLayout(m_contentWidget);
    // 关键设置：确保网格占满水平空间，使用固定列数
    m_gridLayout->setSpacing(8);
    // 使用固定的列数
    m_gridLayout->setColumnStretch(0, 1);
    // 减小外边距使图标靠近容器边缘
    m_gridLayout->setContentsMargins(4, 4, 4, 4);

    // 确保网格布局充满整个宽度
    for (int i = 0; i < GRID_COLUMNS; ++i) {
        m_gridLayout->setColumnStretch(i, 1);
    }

    m_scrollArea->setWidget(m_contentWidget);
    mainLayout->addWidget(m_scrollArea, 1);

    // 分页控制
    QHBoxLayout* pageControlLayout = new QHBoxLayout();
    m_prevButton                   = new NPushButton("上一页", this);
    m_nextButton                   = new NPushButton("下一页", this);
    m_pageLabel                    = new QLabel("页 1/1", this);

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

    // 状态栏
    m_statusLabel = new QLabel("正在加载图标...", this);
    mainLayout->addWidget(m_statusLabel);
}

void IconExample::onIconTypeChanged(int id) {
    m_iconFilterType = static_cast<IconFilterType>(id);
    applyFilter();
}

void IconExample::loadIcons() {
    // 清空现有图标列表
    m_allIcons.clear();

    // 使用元对象系统获取 Regular 图标枚举
    const QMetaEnum regularEnum = QMetaEnum::fromType<NRegularIconType::Icon>();
    if (!regularEnum.isValid()) {
        qWarning() << "无法获取Regular图标元对象信息!";
        return;
    }
    for (int i = 0; i < regularEnum.keyCount(); ++i) {
        const char* key   = regularEnum.key(i);
        int         value = regularEnum.value(i);

        // 跳过无效、特殊值或超出QChar范围的值
        if (value <= 0 || value > 0xffff)
            continue;

        IconInfo info;
        info.name     = QString(key);
        info.code     = static_cast<quint32>(value);
        info.isFilled = false;
        m_allIcons.append(info);
    }

    // 使用元对象系统获取 Filled 图标枚举
    const QMetaEnum filledEnum = QMetaEnum::fromType<NFilledIconType::Icon>();
    if (!filledEnum.isValid()) {
        qWarning() << "无法获取Filled图标元对象信息!";
        return;
    }
    for (int i = 0; i < filledEnum.keyCount(); ++i) {
        const char* key   = filledEnum.key(i);
        int         value = filledEnum.value(i);

        // 跳过无效、特殊值或超出QChar范围的值
        if (value <= 0 || value > 0xffff)
            continue;

        IconInfo info;
        info.name     = QString(key);
        info.code     = static_cast<quint32>(value);
        info.isFilled = true;
        m_allIcons.append(info);
    }

    // 初始加载所有图标
    m_filteredIcons = m_allIcons;
    m_currentPage   = 0;

    // 加载第一页
    loadIconsForPage(0);

    m_statusLabel->setText(QString("共 %1 个图标").arg(m_allIcons.size()));
}

void IconExample::loadIconsForPage(int page) {
    // 清除现有按钮
    for (auto button : m_currentButtons.values()) {
        m_gridLayout->removeWidget(button);
        delete button;
    }
    m_currentButtons.clear();

    // 计算分页
    int totalIcons = m_filteredIcons.size();
    int totalPages = (totalIcons + ICONS_PER_PAGE - 1) / ICONS_PER_PAGE;

    if (page >= totalPages) {
        page = totalPages > 0 ? totalPages - 1 : 0;
    }
    m_currentPage = page;

    // 更新分页控件
    m_pageLabel->setText(QString("页 %1/%2").arg(page + 1).arg(totalPages));
    m_prevButton->setEnabled(page > 0);
    m_nextButton->setEnabled(page < totalPages - 1);

    // 计算当前页的图标范围
    int startIndex = page * ICONS_PER_PAGE;
    int endIndex   = qMin(startIndex + ICONS_PER_PAGE, totalIcons);

    // 使用固定列数布局
    int row = 0, col = 0;
    for (int i = startIndex; i < endIndex; i++) {
        const IconInfo& info = m_filteredIcons[i];

        IconButton* button = new IconButton(info.code, info.name, info.isFilled, m_contentWidget);
        connect(button, &NPushButton::clicked, this, &IconExample::onIconClicked);

        m_gridLayout->addWidget(button, row, col);
        m_currentButtons[i - startIndex] = button;

        col++;
        if (col >= GRID_COLUMNS) {
            col = 0;
            row++;
        }
    }

    // 更新状态信息
    m_statusLabel->setText(QString("显示 %1-%2/%3 个图标").arg(startIndex + 1).arg(endIndex).arg(totalIcons));
}

void IconExample::onSearchTextChanged() {
    // 停止之前的计时器
    m_searchTimer->stop();
    // 启动新的计时器
    m_searchTimer->start();
}

void IconExample::applyFilter() {
    m_searchText = m_searchEdit->text().trimmed();
    m_filteredIcons.clear();

    // 按类型和搜索文本同时筛选
    for (const IconInfo& info : m_allIcons) {
        // 首先按类型筛选
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

        // 然后按搜索文本筛选
        bool textMatch = m_searchText.isEmpty() || info.name.contains(m_searchText, Qt::CaseInsensitive);

        // 两者都匹配才添加到过滤结果中
        if (typeMatch && textMatch) {
            m_filteredIcons.append(info);
        }
    }

    // 重置到第一页并加载
    m_currentPage = 0;
    loadIconsForPage(0);
}

void IconExample::onIconClicked() {
    IconButton* button = qobject_cast<IconButton*>(sender());
    if (button) {
        copyIconCode(button);
    }
}

void IconExample::copyIconCode(IconButton* button) {
    QString enumName;

    if (button->isFilled()) {
        enumName = QString("NFilledIconType::%1").arg(button->iconName());
    } else {
        enumName = QString("NRegularIconType::%1").arg(button->iconName());
    }

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(enumName);

    // 更新状态栏
    m_statusLabel->setText(QString("已复制: %1").arg(enumName));

    // 显示工具提示提供即时反馈
    QToolTip::showText(QCursor::pos(), QString("已复制: %1").arg(enumName), this);
    QTimer::singleShot(2000, []() { QToolTip::hideText(); });
}