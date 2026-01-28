#include "InfoBarPage.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NInfoBar.h>
#include <QtNativeUI/NPushButton.h>

InfoBarPage::InfoBarPage(QWidget* parent)
    : BasePage("InfoBar 信息栏", "用于显示应用级别的通知消息，支持多种类型和位置。", parent) {

    QGridLayout* typeGrid = new QGridLayout();
    typeGrid->setSpacing(16);

    NPushButton* infoButton = new NPushButton("信息");
    infoButton->setFixedSize(120, 40);
    connect(infoButton, &NPushButton::clicked, this, [this]() {
        QString title   = "信息提示";
        QString message = "这是一条普通信息提示，将在3秒后自动关闭";
        NInfoBar::information(NInfoBarType::Top, title, message, 3000, this);
    });

    NPushButton* successButton = new NPushButton("成功");
    successButton->setFixedSize(120, 40);
    connect(successButton, &NPushButton::clicked, this, [this]() {
        QString title   = "操作成功";
        QString message = "您的操作已成功完成，系统已保存相关数据";
        NInfoBar::success(NInfoBarType::Top, title, message, 3000, this);
    });

    NPushButton* warningButton = new NPushButton("警告");
    warningButton->setFixedSize(120, 40);
    connect(warningButton, &NPushButton::clicked, this, [this]() {
        QString title   = "警告";
        QString message = "系统检测到潜在风险，请注意数据安全";
        NInfoBar::warning(NInfoBarType::Top, title, message, 3000, this);
    });

    NPushButton* errorButton = new NPushButton("错误");
    errorButton->setFixedSize(120, 40);
    connect(errorButton, &NPushButton::clicked, this, [this]() {
        QString title   = "错误";
        QString message = "操作失败，请检查输入并重试";
        NInfoBar::error(NInfoBarType::Top, title, message, 3000, this, true);
    });

    typeGrid->addWidget(infoButton, 0, 0);
    typeGrid->addWidget(successButton, 0, 1);
    typeGrid->addWidget(warningButton, 1, 0);
    typeGrid->addWidget(errorButton, 1, 1);

    addSection("不同类型", typeGrid);

    QGridLayout* positionGrid = new QGridLayout();
    positionGrid->setSpacing(16);

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

    int row = 0;
    int col = 0;

    for (const auto& pos : positions) {
        NPushButton* button = new NPushButton(pos.name);
        button->setFixedSize(120, 40);

        connect(button, &NPushButton::clicked, this, [this, pos]() {
            QString title   = QString("%1位置").arg(pos.name);
            QString message = QString("这是显示在%1的InfoBar").arg(pos.name);
            NInfoBar::information(pos.position, title, message, 3000, this);
        });

        positionGrid->addWidget(button, row, col);

        col++;
        if (col >= 4) {
            col = 0;
            row++;
        }
    }

    addSection("不同位置", positionGrid);

    QHBoxLayout* durationLayout = new QHBoxLayout();
    durationLayout->setSpacing(16);

    NPushButton* shortButton = new NPushButton("短时间(1秒)");
    shortButton->setFixedSize(140, 40);
    connect(shortButton, &NPushButton::clicked, this, [this]() {
        QString title   = "短暂提示";
        QString message = "这条消息将在1秒后消失";
        NInfoBar::success(NInfoBarType::TopRight, title, message, 1000, this);
    });

    NPushButton* longButton = new NPushButton("长时间(10秒)");
    longButton->setFixedSize(140, 40);
    connect(longButton, &NPushButton::clicked, this, [this]() {
        QString title   = "长时间提示";
        QString message = "这条消息将在10秒后消失，或者您可以点击关闭按钮";
        NInfoBar::warning(NInfoBarType::TopLeft, title, message, 10000, this);
    });

    NPushButton* noAutoCloseButton = new NPushButton("不自动关闭");
    noAutoCloseButton->setFixedSize(140, 40);
    connect(noAutoCloseButton, &NPushButton::clicked, this, [this]() {
        QString title   = "重要通知";
        QString message = "这条消息不会自动关闭，必须手动点击关闭";
        NInfoBar::error(NInfoBarType::Bottom, title, message, 0, this);
    });

    durationLayout->addWidget(shortButton);
    durationLayout->addWidget(longButton);
    durationLayout->addWidget(noAutoCloseButton);
    durationLayout->addStretch();

    addSection("自定义显示时间", durationLayout);

    QHBoxLayout* stackLayout = new QHBoxLayout();
    stackLayout->setSpacing(16);

    NPushButton* stackButton = new NPushButton("顶部堆叠");
    stackButton->setFixedSize(140, 40);
    connect(stackButton, &NPushButton::clicked, this, [this]() {
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

    NPushButton* stackBottomButton = new NPushButton("底部堆叠");
    stackBottomButton->setFixedSize(140, 40);
    connect(stackBottomButton, &NPushButton::clicked, this, [this]() {
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

    stackLayout->addWidget(stackButton);
    stackLayout->addWidget(stackBottomButton);
    stackLayout->addStretch();

    addSection("多个InfoBar堆叠", stackLayout);

    QVBoxLayout* staticLayout = new QVBoxLayout();
    staticLayout->setSpacing(8);

    NInfoBar* infoInfoBar =
        new NInfoBar(NInfoBarType::Information, "信息", "这是一条信息提示，显示在标题右侧", this);
    staticLayout->addWidget(infoInfoBar);

    NInfoBar* successInfoBar = new NInfoBar(NInfoBarType::Success, "成功", "操作已成功完成", this);
    staticLayout->addWidget(successInfoBar);

    NInfoBar* warningInfoBar = new NInfoBar(NInfoBarType::Warning, "警告", "检测到潜在风险", this);
    staticLayout->addWidget(warningInfoBar);

    NInfoBar* errorInfoBar = new NInfoBar(NInfoBarType::Error, "错误", "操作失败，请重试", this);
    staticLayout->addWidget(errorInfoBar);

    NInfoBar* longTextInfoBar = new NInfoBar(
        NInfoBarType::Information,
        "长文本示例",
        "这是一段较长的文本内容，它将自动换行显示在标题的下方。这种设计符合 Fluent Design 的规范，提高了可读性。",
        this);
    staticLayout->addWidget(longTextInfoBar);

    NInfoBar* withButtonInfoBar = new NInfoBar(NInfoBarType::Warning, "需要操作", "此操作需要确认", this);
    NPushButton* confirmButton  = new NPushButton("确认", withButtonInfoBar);
    NPushButton* cancelButton   = new NPushButton("取消", withButtonInfoBar);
    confirmButton->setFixedSize(80, 32);
    cancelButton->setFixedSize(80, 32);
    withButtonInfoBar->addWidget(confirmButton);
    withButtonInfoBar->addWidget(cancelButton);
    staticLayout->addWidget(withButtonInfoBar);

    addSection("直接创建 InfoBar widget", staticLayout);
}
