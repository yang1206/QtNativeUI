#include "dialogs.h"

#include <QApplication>
#include <QColor>
#include <QContextMenuEvent>
#include <QDebug>
#include <QFormLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QtNativeUI/NFlyout.h>
#include <QtNativeUI/NInfoBar.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NScrollArea.h>
#include <QtNativeUI/NSpinBox.h>

#include <QtNativeUI/NContentDialog.h>
#include "QtNativeUI/NCalendarWidget.h"
#include "widgets/ExampleSection.h"

DialogsExample::DialogsExample(QWidget* parent) : QWidget(parent) { initUI(); }

void DialogsExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建滚动区域
    m_scrollArea = new NScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建内容容器
    QWidget*     contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    // 添加各个对话框控件区域
    contentLayout->addWidget(new ExampleSection("InfoBar", createInfoBars()));
    contentLayout->addWidget(new ExampleSection("Flyout", createFlyouts()));
    contentLayout->addWidget(new ExampleSection("Content Dialog", createContentDialogs()));

    contentLayout->addStretch();

    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);
    setMinimumWidth(600);
}

QWidget* DialogsExample::createInfoBars() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(24);

    // 基本InfoBar示例
    QLabel* basicInfoBarLabel = new QLabel("基本InfoBar:", container);
    QFont   labelFont         = basicInfoBarLabel->font();
    labelFont.setBold(true);
    basicInfoBarLabel->setFont(labelFont);
    layout->addWidget(basicInfoBarLabel);

    // 不同类型的InfoBar
    QGridLayout* typeGrid = new QGridLayout();
    typeGrid->setSpacing(20);

    // 信息类型
    NPushButton* infoButton = new NPushButton("显示信息InfoBar", container);
    connect(infoButton, &NPushButton::clicked, [this]() {
        QString title   = "信息提示";
        QString message = "这是一条普通信息提示，将在3秒后自动关闭";
        NInfoBar::information(NInfoBarType::Top, title, message, 3000, this);
    });
    typeGrid->addWidget(infoButton, 0, 0);

    // 成功类型
    NPushButton* successButton = new NPushButton("显示成功InfoBar", container);
    connect(successButton, &NPushButton::clicked, [this]() {
        QString title   = "操作成功";
        QString message = "您的操作已成功完成，系统已保存相关数据";
        NInfoBar::success(NInfoBarType::Top, title, message, 3000, this);
    });
    typeGrid->addWidget(successButton, 0, 1);

    // 警告类型
    NPushButton* warningButton = new NPushButton("显示警告InfoBar", container);
    connect(warningButton, &NPushButton::clicked, [this]() {
        QString title   = "警告";
        QString message = "系统检测到潜在风险，请注意数据安全";
        NInfoBar::warning(NInfoBarType::Top, title, message, 3000, this);
    });
    typeGrid->addWidget(warningButton, 1, 0);

    // 错误类型
    NPushButton* errorButton = new NPushButton("显示错误InfoBar", container);
    connect(errorButton, &NPushButton::clicked, [this]() {
        QString title   = "错误";
        QString message = "操作失败，请检查输入并重试";
        NInfoBar::error(NInfoBarType::Top, title, message, 3000, this, false);
    });
    typeGrid->addWidget(errorButton, 1, 1);

    layout->addLayout(typeGrid);

    // 不同位置的InfoBar
    QLabel* positionLabel = new QLabel("不同位置的InfoBar:", container);
    positionLabel->setFont(labelFont);
    layout->addWidget(positionLabel);

    QGridLayout* positionGrid = new QGridLayout();
    positionGrid->setSpacing(20);

    // 定义位置数组
    struct InfoBarPosition {
        QString                      name;
        NInfoBarType::PositionPolicy position;
    };

    QList<InfoBarPosition> positions = {{"顶部", NInfoBarType::Top},
                                        {"顶部左侧", NInfoBarType::TopLeft},
                                        {"顶部右侧", NInfoBarType::TopRight},
                                        {"底部", NInfoBarType::Bottom},
                                        {"底部左侧", NInfoBarType::BottomLeft},
                                        {"底部右侧", NInfoBarType::BottomRight},
                                        {"左侧", NInfoBarType::Left},
                                        {"右侧", NInfoBarType::Right}};

    int row    = 0;
    int col    = 0;
    int maxCol = 4;

    for (const auto& pos : positions) {
        NPushButton* button = new NPushButton(QString("%1位置").arg(pos.name), container);

        connect(button, &NPushButton::clicked, [this, pos]() {
            QString title   = QString("%1位置").arg(pos.name);
            QString message = QString("这是显示在%1的InfoBar").arg(pos.name);
            NInfoBar::information(pos.position, title, message, 3000, this);
        });

        positionGrid->addWidget(button, row, col);

        col++;
        if (col >= maxCol) {
            col = 0;
            row++;
        }
    }

    layout->addLayout(positionGrid);

    // 自定义显示时间
    QLabel* durationLabel = new QLabel("自定义显示时间:", container);
    durationLabel->setFont(labelFont);
    layout->addWidget(durationLabel);

    QHBoxLayout* durationLayout = new QHBoxLayout();

    // 短时间显示
    NPushButton* shortButton = new NPushButton("短时间显示(1秒)", container);
    connect(shortButton, &NPushButton::clicked, [this]() {
        QString title   = "短暂提示";
        QString message = "这条消息将在1秒后消失";
        NInfoBar::success(NInfoBarType::TopRight, title, message, 1000, this);
    });
    durationLayout->addWidget(shortButton);

    // 长时间显示
    NPushButton* longButton = new NPushButton("长时间显示(10秒)", container);
    connect(longButton, &NPushButton::clicked, [this]() {
        QString title   = "长时间提示";
        QString message = "这条消息将在10秒后消失，或者您可以点击关闭按钮";
        NInfoBar::warning(NInfoBarType::TopLeft, title, message, 10000, this);
    });
    durationLayout->addWidget(longButton);

    // 不自动关闭
    NPushButton* noAutoCloseButton = new NPushButton("不自动关闭", container);
    connect(noAutoCloseButton, &NPushButton::clicked, [this]() {
        QString title   = "重要通知";
        QString message = "这条消息不会自动关闭，必须手动点击关闭";
        NInfoBar::error(NInfoBarType::Bottom, title, message, 0, this);
    });
    durationLayout->addWidget(noAutoCloseButton);

    durationLayout->addStretch();
    layout->addLayout(durationLayout);

    // 多个InfoBar堆叠
    QLabel* stackLabel = new QLabel("多个InfoBar堆叠:", container);
    stackLabel->setFont(labelFont);
    layout->addWidget(stackLabel);

    QHBoxLayout* stackLayout = new QHBoxLayout();

    NPushButton* stackButton = new NPushButton("显示多个InfoBar", container);
    connect(stackButton, &NPushButton::clicked, [this]() {
        // 显示多个不同类型的InfoBar，它们会自动堆叠
        QString title1   = "第一条消息";
        QString message1 = "这是第一条消息，显示在顶部";
        NInfoBar::information(NInfoBarType::Top, title1, message1, 5000, this);

        QString title2   = "第二条消息";
        QString message2 = "这是第二条消息，也显示在顶部，会自动堆叠";
        NInfoBar::success(NInfoBarType::Top, title2, message2, 8000, this);

        QString title3   = "第三条消息";
        QString message3 = "这是第三条消息，同样显示在顶部，会继续堆叠";
        NInfoBar::warning(NInfoBarType::Top, title3, message3, 10000, this);
    });
    stackLayout->addWidget(stackButton);

    NPushButton* stackBottomButton = new NPushButton("在底部堆叠InfoBar", container);
    connect(stackBottomButton, &NPushButton::clicked, [this]() {
        // 显示多个不同类型的InfoBar在底部
        QString title1   = "底部消息1";
        QString message1 = "这是底部的第一条消息";
        NInfoBar::information(NInfoBarType::Bottom, title1, message1, 5000, this);

        QString title2   = "底部消息2";
        QString message2 = "这是底部的第二条消息，会自动堆叠";
        NInfoBar::success(NInfoBarType::Bottom, title2, message2, 8000, this);

        QString title3   = "底部消息3";
        QString message3 = "这是底部的第三条消息，继续堆叠";
        NInfoBar::warning(NInfoBarType::Bottom, title3, message3, 10000, this);
    });
    stackLayout->addWidget(stackBottomButton);

    stackLayout->addStretch();
    layout->addLayout(stackLayout);

    // 直接创建 InfoBar widget 示例
    QLabel* staticInfoBarLabel = new QLabel("直接创建 InfoBar widget:", container);
    staticInfoBarLabel->setFont(labelFont);
    layout->addWidget(staticInfoBarLabel);

    QVBoxLayout* staticInfoBarLayout = new QVBoxLayout();

    // 创建一个容器用于放置 InfoBar
    QWidget*     infoBarContainer       = new QWidget(container);
    QVBoxLayout* infoBarContainerLayout = new QVBoxLayout(infoBarContainer);
    infoBarContainerLayout->setContentsMargins(0, 10, 0, 10);

    // 创建不同类型的 InfoBar
    NInfoBar* infoInfoBar =
        new NInfoBar(NInfoBarType::Information, "信息", "这是一条信息提示，显示在标题右侧", infoBarContainer);
    infoBarContainerLayout->addWidget(infoInfoBar);

    NInfoBar* successInfoBar = new NInfoBar(NInfoBarType::Success, "成功", "操作已成功完成", infoBarContainer);
    infoBarContainerLayout->addWidget(successInfoBar);

    NInfoBar* warningInfoBar = new NInfoBar(NInfoBarType::Warning, "警告", "检测到潜在风险", infoBarContainer);
    infoBarContainerLayout->addWidget(warningInfoBar);

    NInfoBar* errorInfoBar = new NInfoBar(NInfoBarType::Error, "错误", "操作失败，请重试", infoBarContainer);
    infoBarContainerLayout->addWidget(errorInfoBar);

    // 长文本示例
    NInfoBar* longTextInfoBar = new NInfoBar(
        NInfoBarType::Information,
        "长文本示例",
        "这是一段较长的文本内容，它将自动换行显示在标题的下方。这种设计符合 Fluent Design 的规范，提高了可读性。",
        infoBarContainer);
    infoBarContainerLayout->addWidget(longTextInfoBar);

    // 添加按钮的示例
    NInfoBar* withButtonInfoBar = new NInfoBar(NInfoBarType::Warning, "需要操作", "此操作需要确认", infoBarContainer);
    NPushButton* confirmButton  = new NPushButton("确认", withButtonInfoBar);
    NPushButton* cancelButton   = new NPushButton("取消", withButtonInfoBar);
    withButtonInfoBar->addWidget(confirmButton);
    withButtonInfoBar->addWidget(cancelButton);
    infoBarContainerLayout->addWidget(withButtonInfoBar);

    // 添加到布局
    staticInfoBarLayout->addWidget(infoBarContainer);
    layout->addLayout(staticInfoBarLayout);

    // 动态创建和添加 InfoBar 的示例
    QHBoxLayout* dynamicInfoBarLayout = new QHBoxLayout();
    NPushButton* addInfoBarButton     = new NPushButton("添加新的 InfoBar", container);

    QWidget*     dynamicContainer       = new QWidget(container);
    QVBoxLayout* dynamicContainerLayout = new QVBoxLayout(dynamicContainer);
    dynamicContainerLayout->setContentsMargins(0, 10, 0, 10);

    connect(addInfoBarButton, &NPushButton::clicked, [=]() {
        static int count      = 1;
        NInfoBar*  newInfoBar = new NInfoBar(count % 4 == 0   ? NInfoBarType::Error
                                            : count % 4 == 1 ? NInfoBarType::Information
                                            : count % 4 == 2 ? NInfoBarType::Success
                                                             : NInfoBarType::Warning,
                                            QString("动态创建 #%1").arg(count),
                                            QString("这是第 %1 个动态创建的 InfoBar").arg(count),
                                            dynamicContainer);

        // 如果是偶数，添加一个按钮
        if (count % 2 == 0) {
            NPushButton* actionButton = new NPushButton("操作", newInfoBar);
            connect(actionButton, &NPushButton::clicked, [=]() { newInfoBar->deleteLater(); });
            newInfoBar->addWidget(actionButton);
        }

        dynamicContainerLayout->addWidget(newInfoBar);
        count++;
    });

    dynamicInfoBarLayout->addWidget(addInfoBarButton);
    dynamicInfoBarLayout->addStretch();
    layout->addLayout(dynamicInfoBarLayout);
    layout->addWidget(dynamicContainer);

    layout->addStretch();
    return container;
}

QWidget* DialogsExample::createFlyouts() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(24);

    // 基本Flyout示例
    QLabel* basicFlyoutLabel = new QLabel("基本Flyout:", container);
    QFont   labelFont        = basicFlyoutLabel->font();
    labelFont.setBold(true);
    basicFlyoutLabel->setFont(labelFont);
    layout->addWidget(basicFlyoutLabel);

    // 不同位置的Flyouts
    QGridLayout* positionGrid = new QGridLayout();
    positionGrid->setSpacing(20);

    // 定义位置数组
    struct FlyoutPosition {
        QString  name;
        Qt::Edge position;
    };

    QList<FlyoutPosition> positions = {
        {"顶部", Qt::TopEdge}, {"底部", Qt::BottomEdge}, {"左侧", Qt::LeftEdge}, {"右侧", Qt::RightEdge}};

    int row    = 0;
    int col    = 0;
    int maxCol = 4;

    for (const auto& pos : positions) {
        NPushButton* button = new NPushButton(QString("显示%1 Flyout").arg(pos.name), container);

        connect(button, &NPushButton::clicked, [this, button, pos]() {
            // 创建内容
            QWidget*     content       = new QWidget();
            QVBoxLayout* contentLayout = new QVBoxLayout(content);

            QLabel* titleLabel = new QLabel(QString("%1位置的Flyout").arg(pos.name), content);
            QFont   titleFont  = titleLabel->font();
            titleFont.setBold(true);
            titleLabel->setFont(titleFont);

            QLabel* descLabel = new QLabel("这是Flyout的内容区域，可以放置任意组件", content);
            descLabel->setWordWrap(true);

            NPushButton* closeButton = new NPushButton("关闭", content);

            contentLayout->addWidget(titleLabel);
            contentLayout->addWidget(descLabel);
            contentLayout->addWidget(closeButton);

            content->setMinimumWidth(200);

            // 创建Flyout
            NFlyout* flyout = new NFlyout(this);
            flyout->setContent(content);
            flyout->setAttribute(Qt::WA_DeleteOnClose);

            // 设置位置属性 - 注意顺序，先设置属性，再调用showAt
            flyout->setPlacement(pos.position);

            // 根据位置设置对应的动画类型
            NFlyoutAnimationType animType;
            switch (pos.position) {
                case Qt::TopEdge:
                    animType = NFlyoutAnimationType::DROP_DOWN;
                    break;
                case Qt::BottomEdge:
                    animType = NFlyoutAnimationType::PULL_UP;
                    break;
                case Qt::LeftEdge:
                    animType = NFlyoutAnimationType::SLIDE_RIGHT;
                    break;
                case Qt::RightEdge:
                    animType = NFlyoutAnimationType::SLIDE_LEFT;
                    break;
                default:
                    animType = NFlyoutAnimationType::FADE_IN;
            }
            flyout->setAnimationType(animType);

            connect(closeButton, &NPushButton::clicked, flyout, &NFlyout::hide);

            // 最后调用showAt显示
            flyout->showAt(button);
        });

        positionGrid->addWidget(button, row, col);

        col++;
        if (col >= maxCol) {
            col = 0;
            row++;
        }
    }
    layout->addLayout(positionGrid);

    // 自定义内容的Flyout
    QLabel* customContentLabel = new QLabel("自定义内容的Flyout:", container);
    customContentLabel->setFont(labelFont);
    layout->addWidget(customContentLabel);

    QHBoxLayout* customContentLayout = new QHBoxLayout();

    // 带表单的Flyout
    NPushButton* formFlyoutButton = new NPushButton("显示带表单的Flyout", container);
    connect(formFlyoutButton, &NPushButton::clicked, [this, formFlyoutButton]() {
        // 创建表单内容
        QWidget*     formContent   = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(formContent);

        QLabel* titleLabel = new QLabel("用户信息", formContent);
        QFont   titleFont  = titleLabel->font();
        titleFont.setBold(true);

        int newPointSize = titleFont.pointSize();
        if (newPointSize <= 0) {
            titleFont.setPointSize(10);
        } else {
            titleFont.setPointSize(newPointSize + 1);
        }

        titleLabel->setFont(titleFont);

        QFormLayout* formLayout = new QFormLayout();
        NLineEdit*   nameEdit   = new NLineEdit(formContent);
        NLineEdit*   emailEdit  = new NLineEdit(formContent);
        NSpinBox*    ageSpinBox = new NSpinBox(formContent);
        ageSpinBox->setMinimumWidth(100);
        ageSpinBox->setRange(1, 120);
        ageSpinBox->setValue(30);

        formLayout->addRow("姓名:", nameEdit);
        formLayout->addRow("邮箱:", emailEdit);
        formLayout->addRow("年龄:", ageSpinBox);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        NPushButton* submitButton = new NPushButton("提交", formContent);
        NPushButton* cancelButton = new NPushButton("取消", formContent);

        buttonLayout->addWidget(submitButton);
        buttonLayout->addWidget(cancelButton);

        contentLayout->addWidget(titleLabel);
        contentLayout->addLayout(formLayout);
        contentLayout->addSpacing(10);
        contentLayout->addLayout(buttonLayout);

        formContent->setMinimumWidth(300);

        // 创建Flyout
        NFlyout* flyout = NFlyout::createWithContent(formContent, formFlyoutButton, this);

        flyout->setAttribute(Qt::WA_DeleteOnClose);
        flyout->setPlacement(Qt::RightEdge);

        connect(submitButton, &NPushButton::clicked, [=]() {
            qDebug() << "提交表单:" << nameEdit->text() << emailEdit->text() << ageSpinBox->value();
            flyout->hide();
        });
        connect(cancelButton, &NPushButton::clicked, flyout, &NFlyout::hide);

        flyout->showAt(formFlyoutButton);
    });
    customContentLayout->addWidget(formFlyoutButton);

    // 带列表的Flyout
    NPushButton* listFlyoutButton = new NPushButton("显示带列表的Flyout", container);

    connect(listFlyoutButton, &NPushButton::clicked, [this, listFlyoutButton]() {
        // 创建列表内容
        QWidget*     listContent   = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(listContent);

        QLabel* titleLabel = new QLabel("选择项目", listContent);
        QFont   titleFont  = titleLabel->font();
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);

        QListWidget* listWidget = new QListWidget(listContent);
        listWidget->setMinimumHeight(200);

        for (int i = 1; i <= 10; i++) {
            QListWidgetItem* item = new QListWidgetItem(QString("项目 %1").arg(i), listWidget);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
        }

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        NPushButton* okButton     = new NPushButton("确定", listContent);
        NPushButton* cancelButton = new NPushButton("取消", listContent);

        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);

        contentLayout->addWidget(titleLabel);
        contentLayout->addWidget(listWidget);
        contentLayout->addLayout(buttonLayout);

        listContent->setMinimumSize(250, 300);

        // 创建Flyout
        NFlyout* flyout = new NFlyout(this);
        flyout->setContent(listContent);
        flyout->setContentsMargins(5, 5, 5, 5);
        flyout->setAttribute(Qt::WA_DeleteOnClose);
        flyout->setPlacement(Qt::BottomEdge);

        connect(okButton, &NPushButton::clicked, [=]() {
            QStringList selected;
            for (int i = 0; i < listWidget->count(); i++) {
                if (listWidget->item(i)->checkState() == Qt::Checked) {
                    selected.append(listWidget->item(i)->text());
                }
            }
            qDebug() << "已选择项目:" << selected;
            flyout->hide();
        });
        connect(cancelButton, &NPushButton::clicked, flyout, &NFlyout::hide);

        flyout->showAt(listFlyoutButton);
    });
    customContentLayout->addWidget(listFlyoutButton);

    // calendar的Flyout
    NPushButton* calendarFlyoutButton = new NPushButton("显示日历视图的Flyout", container);

    connect(calendarFlyoutButton, &NPushButton::clicked, [this, calendarFlyoutButton]() {
        // 创建列表内容
        QWidget*         calendarContent = new QWidget();
        QVBoxLayout*     contentLayout   = new QVBoxLayout(calendarContent);
        NCalendarWidget* calendarWidget  = new NCalendarWidget(calendarContent);
        contentLayout->addWidget(calendarWidget);

        calendarContent->setMinimumSize(250, 300);

        // 创建Flyout
        NFlyout* flyout = new NFlyout(this);
        flyout->setContent(calendarContent);
        flyout->setContentsMargins(0, 0, 0, 0);
        flyout->setAttribute(Qt::WA_DeleteOnClose);
        flyout->setPlacement(Qt::BottomEdge);

        flyout->showAt(calendarFlyoutButton);
    });
    customContentLayout->addWidget(calendarFlyoutButton);
    customContentLayout->addStretch();

    layout->addLayout(customContentLayout);

    // 自定义样式的Flyout
    QLabel* styleLabel = new QLabel("自定义样式:", container);
    styleLabel->setFont(labelFont);
    layout->addWidget(styleLabel);

    QHBoxLayout* styleLayout = new QHBoxLayout();
    NPushButton* styleButton = new NPushButton("显示自定义样式的Flyout", container);

    connect(styleButton, &NPushButton::clicked, [this, styleButton]() {
        // 创建内容
        QWidget*     content       = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(content);

        QLabel* titleLabel = new QLabel("自定义样式", content);
        QFont   titleFont  = titleLabel->font();
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);

        QLabel* descLabel = new QLabel("这个Flyout使用了自定义的背景色、边框色和圆角", content);
        descLabel->setWordWrap(true);

        NPushButton* closeButton = new NPushButton("关闭", content);

        contentLayout->addWidget(titleLabel);
        contentLayout->addWidget(descLabel);
        contentLayout->addWidget(closeButton);

        content->setMinimumSize(250, 150);

        // 创建Flyout
        NFlyout* flyout = new NFlyout(this);
        flyout->setContent(content);
        flyout->setAttribute(Qt::WA_DeleteOnClose);

        // 自定义样式
        flyout->setLightBackgroundColor(QColor(240, 240, 255));
        flyout->setDarkBackgroundColor(QColor(30, 30, 45));
        flyout->setLightBorderColor(QColor(180, 180, 255));
        flyout->setDarkBorderColor(QColor(60, 60, 90));
        flyout->setBorderRadius(12);
        flyout->setBorderWidth(2);

        connect(closeButton, &NPushButton::clicked, flyout, &NFlyout::hide);

        flyout->showAt(styleButton);
    });

    styleLayout->addWidget(styleButton);
    styleLayout->addStretch();
    layout->addLayout(styleLayout);

    // Light Dismiss 模式
    QLabel* dismissLabel = new QLabel("Light Dismiss模式:", container);
    dismissLabel->setFont(labelFont);
    layout->addWidget(dismissLabel);

    QHBoxLayout* dismissLayout = new QHBoxLayout();
    NPushButton* onButton      = new NPushButton("强制启用Light Dismiss", container);
    NPushButton* offButton     = new NPushButton("强制禁用Light Dismiss", container);

    connect(onButton, &NPushButton::clicked, [this, onButton]() {
        // 创建内容
        QWidget*     content       = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(content);

        QLabel* titleLabel = new QLabel("启用Light Dismiss", content);
        QFont   titleFont  = titleLabel->font();
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);

        QLabel* descLabel = new QLabel("点击Flyout外部区域将关闭此Flyout", content);
        descLabel->setWordWrap(true);

        contentLayout->addWidget(titleLabel);
        contentLayout->addWidget(descLabel);
        contentLayout->addStretch();

        content->setMinimumSize(250, 150);

        // 创建Flyout
        NFlyout* flyout = new NFlyout(this);
        flyout->setContent(content);
        flyout->setAttribute(Qt::WA_DeleteOnClose);
        flyout->setLightDismissMode(NFlyout::On);

        flyout->showAt(onButton);
    });

    connect(offButton, &NPushButton::clicked, [this, offButton]() {
        // 创建内容
        QWidget*     content       = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(content);

        QLabel* titleLabel = new QLabel("禁用Light Dismiss", content);
        QFont   titleFont  = titleLabel->font();
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);

        QLabel* descLabel = new QLabel("点击Flyout外部区域不会关闭此Flyout，必须通过按钮关闭", content);
        descLabel->setWordWrap(true);

        NPushButton* closeButton = new NPushButton("关闭", content);

        contentLayout->addWidget(titleLabel);
        contentLayout->addWidget(descLabel);
        contentLayout->addWidget(closeButton);

        content->setMinimumSize(250, 150);

        // 创建Flyout
        NFlyout* flyout = new NFlyout(this);
        flyout->setContent(content);
        flyout->setAttribute(Qt::WA_DeleteOnClose);

        // 重要：先设置属性
        flyout->setLightDismissMode(NFlyout::Off);

        // 然后连接信号
        connect(closeButton, &NPushButton::clicked, flyout, &NFlyout::hide);

        // 最后显示
        flyout->showAt(offButton);
    });

    dismissLayout->addWidget(onButton);
    dismissLayout->addWidget(offButton);
    dismissLayout->addStretch();
    layout->addLayout(dismissLayout);

    layout->addStretch();
    return container;
}

QWidget* DialogsExample::createContentDialogs() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(24);

    // 基本 ContentDialog 示例
    QLabel* basicDialogLabel = new QLabel("基本内容对话框:", container);
    QFont   labelFont        = basicDialogLabel->font();
    labelFont.setBold(true);
    basicDialogLabel->setFont(labelFont);
    layout->addWidget(basicDialogLabel);

    // 基本对话框示例
    QHBoxLayout* basicDialogLayout = new QHBoxLayout();
    NPushButton* basicDialogButton = new NPushButton("显示基本对话框", container);

    connect(basicDialogButton, &NPushButton::clicked, [this]() {
        NContentDialog* dialog = new NContentDialog(this);
        dialog->setTitle("确认操作");
        dialog->setContent("您确定要执行此操作吗？此操作不可撤销。");
        dialog->setPrimaryButtonText("确定");
        dialog->setCloseButtonText("取消");

        connect(dialog, &NContentDialog::primaryButtonClicked, [=]() { qDebug() << "用户点击了确定按钮"; });

        connect(dialog, &NContentDialog::secondaryButtonClicked, [=]() { qDebug() << "用户点击了取消按钮"; });

        NContentDialog::DialogResult result = dialog->showDialog();
        qDebug() << "对话框结果:" << result;
    });

    basicDialogLayout->addWidget(basicDialogButton);
    basicDialogLayout->addStretch();
    layout->addLayout(basicDialogLayout);

    // 自定义内容对话框
    QLabel* customDialogLabel = new QLabel("自定义内容对话框:", container);
    customDialogLabel->setFont(labelFont);
    layout->addWidget(customDialogLabel);

    QHBoxLayout* customDialogLayout = new QHBoxLayout();
    NPushButton* customDialogButton = new NPushButton("显示自定义内容对话框", container);

    connect(customDialogButton, &NPushButton::clicked, [this]() {
        NContentDialog* dialog = new NContentDialog(this);
        dialog->setTitle("自定义内容");

        // 创建自定义内容
        QWidget*     customContent = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(customContent);

        QLabel* infoLabel = new QLabel("请输入您的信息:", customContent);

        QFormLayout* formLayout = new QFormLayout();
        NLineEdit*   nameEdit   = new NLineEdit(customContent);
        NLineEdit*   emailEdit  = new NLineEdit(customContent);

        formLayout->addRow("姓名:", nameEdit);
        formLayout->addRow("邮箱:", emailEdit);

        contentLayout->addWidget(infoLabel);
        contentLayout->addLayout(formLayout);

        dialog->setContentWidget(customContent);

        dialog->setPrimaryButtonText("提交");
        dialog->setCloseButtonText("取消");

        connect(dialog, &NContentDialog::primaryButtonClicked, [=]() {
            qDebug() << "提交的信息:" << nameEdit->text() << emailEdit->text();
        });

        NContentDialog::DialogResult result = dialog->showDialog();
        qDebug() << "对话框结果:" << result;
    });

    customDialogLayout->addWidget(customDialogButton);
    customDialogLayout->addStretch();
    layout->addLayout(customDialogLayout);

    // 三按钮对话框
    QLabel* threeButtonLabel = new QLabel("三按钮对话框:", container);
    threeButtonLabel->setFont(labelFont);
    layout->addWidget(threeButtonLabel);

    QHBoxLayout* threeButtonLayout       = new QHBoxLayout();
    NPushButton* threeButtonDialogButton = new NPushButton("显示三按钮对话框", container);

    connect(threeButtonDialogButton, &NPushButton::clicked, [this]() {
        NContentDialog* dialog = new NContentDialog(this);
        dialog->setTitle("保存文档");
        dialog->setContent("是否要保存对文档的更改？");
        dialog->setPrimaryButtonText("保存");
        dialog->setSecondaryButtonText("不保存");
        dialog->setCloseButtonText("取消");

        connect(dialog, &NContentDialog::primaryButtonClicked, [=]() { qDebug() << "用户选择了保存"; });

        connect(dialog, &NContentDialog::secondaryButtonClicked, [=]() { qDebug() << "用户选择了不保存"; });

        connect(dialog, &NContentDialog::closeButtonClicked, [=]() { qDebug() << "用户选择了取消"; });

        NContentDialog::DialogResult result = dialog->showDialog();
        qDebug() << "对话框结果:" << result;
    });

    threeButtonLayout->addWidget(threeButtonDialogButton);
    threeButtonLayout->addStretch();
    layout->addLayout(threeButtonLayout);

    layout->addStretch();
    return container;
}