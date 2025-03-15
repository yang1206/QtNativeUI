#include "icon.h"
#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMetaEnum>
#include <QRegularExpression>
#include <QVBoxLayout>

#include "QtNativeUI/NIcon.h"

IconButton::IconButton(quint32 iconCode, const QString& name, bool isFilled, QWidget* parent)
    : QPushButton(parent), m_iconName(name), m_iconCode(iconCode), m_isFilled(isFilled) {
    setFixedSize(80, 80);
    setToolTip(name);

    // 设置图标
    if (isFilled) {
        setIcon(nIcon->fromFilled(static_cast<NFilledIconType::Icon>(iconCode), 32));
    } else {
        setIcon(nIcon->fromRegular(static_cast<NRegularIconType::Icon>(iconCode), 32));
    }

    setIconSize(QSize(32, 32));

    // 显示名称，截断过长的名称
    QString displayName = name;
    if (displayName.length() > 10) {
        displayName = displayName.left(8) + "...";
    }

    setText(displayName);
    setToolTip(name);
}

IconExample::IconExample(QWidget* parent) : QWidget(parent) {
    initUI();
    // loadIcons();
}

void IconExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 顶部搜索栏
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel*      searchLabel  = new QLabel("搜索:", this);
    m_searchEdit              = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("输入图标名称搜索");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &IconExample::onSearchTextChanged);

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_searchEdit);

    mainLayout->addLayout(searchLayout);

    // 图标显示区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    m_contentWidget = new QWidget(m_scrollArea);
    m_gridLayout    = new QGridLayout(m_contentWidget);
    m_gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_scrollArea->setWidget(m_contentWidget);
    mainLayout->addWidget(m_scrollArea, 1);

    // 分页控制
    QHBoxLayout* pageControlLayout = new QHBoxLayout();
    m_prevButton                   = new QPushButton("上一页", this);
    m_nextButton                   = new QPushButton("下一页", this);
    m_pageLabel                    = new QLabel("页 1/1", this);

    connect(m_prevButton, &QPushButton::clicked, this, [this]() {
        if (m_currentPage > 0) {
            m_currentPage--;
            loadIconsForPage(m_currentPage);
        }
    });

    connect(m_nextButton, &QPushButton::clicked, this, [this]() {
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

// void IconExample::loadIcons() {
//     // 清空现有图标列表
//     m_allIcons.clear();
//
//     // 使用元对象系统获取 Regular 图标枚举
//     const QMetaEnum regularEnum = QMetaEnum::fromType<NRegularIconType::Icon>();
//     for (int i = 0; i < regularEnum.keyCount(); ++i) {
//         const char* key = regularEnum.key(i);
//         int value = regularEnum.value(i);
//
//         // 跳过无效或特殊值
//         if (value <= 0) continue;
//
//         IconInfo info;
//         info.name = QString(key);
//         info.code = static_cast<quint32>(value);
//         info.isFilled = false;
//         m_allIcons.append(info);
//     }
//
//     // 使用元对象系统获取 Filled 图标枚举
//     const QMetaEnum filledEnum = QMetaEnum::fromType<NFilledIconType::Icon>();
//     for (int i = 0; i < filledEnum.keyCount(); ++i) {
//         const char* key = filledEnum.key(i);
//         int value = filledEnum.value(i);
//
//         // 跳过无效或特殊值
//         if (value <= 0) continue;
//
//         IconInfo info;
//         info.name = QString(key);
//         info.code = static_cast<quint32>(value);
//         info.isFilled = true;
//         m_allIcons.append(info);
//     }
//
//     // 初始加载所有图标
//     m_filteredIcons = m_allIcons;
//     m_currentPage = 0;
//
//     // 加载第一页
//     loadIconsForPage(0);
//
//     m_statusLabel->setText(QString("共 %1 个图标").arg(m_allIcons.size()));
// }

void IconExample::loadIconsForPage(int page) {
    // 清除现有内容
    QLayoutItem* item;
    while ((item = m_gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

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

    // 添加图标按钮
    int row = 0, col = 0;
    for (int i = startIndex; i < endIndex; i++) {
        const IconInfo& info = m_filteredIcons[i];

        IconButton* button = new IconButton(info.code, info.name, info.isFilled, m_contentWidget);

        connect(button, &QPushButton::clicked, this, &IconExample::onIconClicked);

        m_gridLayout->addWidget(button, row, col);

        col++;
        if (col >= GRID_COLUMNS) {
            col = 0;
            row++;
        }
    }

    // 更新状态信息
    m_statusLabel->setText(QString("显示 %1-%2/%3 个图标").arg(startIndex + 1).arg(endIndex).arg(totalIcons));
}

void IconExample::onSearchTextChanged(const QString& text) {
    m_searchText = text.trimmed();

    if (m_searchText.isEmpty()) {
        // 如果搜索框为空，显示所有图标
        m_filteredIcons = m_allIcons;
    } else {
        // 根据搜索文本过滤图标
        m_filteredIcons.clear();
        QRegularExpression regex(m_searchText, QRegularExpression::CaseInsensitiveOption);

        for (const IconInfo& info : m_allIcons) {
            if (info.name.contains(regex)) {
                m_filteredIcons.append(info);
            }
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

    m_statusLabel->setText(QString("已复制: %1").arg(enumName));
}