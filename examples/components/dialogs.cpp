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
    contentLayout->addWidget(new ExampleSection("Flyout", createFlyouts()));
    contentLayout->addWidget(new ExampleSection("Content Dialog", createContentDialogs()));

    contentLayout->addStretch();

    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);
    setMinimumWidth(600);
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