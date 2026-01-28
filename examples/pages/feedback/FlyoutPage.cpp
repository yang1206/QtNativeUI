#include "FlyoutPage.h"
#include <QDebug>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QtNativeUI/NCalendarWidget.h>
#include <QtNativeUI/NFlyout.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NSpinBox.h>

FlyoutPage::FlyoutPage(QWidget* parent)
    : BasePage("Flyout 浮出控件", "轻量级弹出面板，可以在任意控件旁边显示内容。", parent) {

    QHBoxLayout* positionLayout = new QHBoxLayout();
    positionLayout->setSpacing(16);

    struct FlyoutPosition {
        QString  name;
        Qt::Edge position;
    };

    QList<FlyoutPosition> positions = {
        {"顶部", Qt::TopEdge}, {"底部", Qt::BottomEdge}, {"左侧", Qt::LeftEdge}, {"右侧", Qt::RightEdge}};

    for (const auto& pos : positions) {
        NPushButton* button = new NPushButton(QString("%1 Flyout").arg(pos.name));
        button->setFixedSize(120, 40);

        connect(button, &NPushButton::clicked, this, [this, button, pos]() {
            QWidget*     content       = new QWidget();
            QVBoxLayout* contentLayout = new QVBoxLayout(content);

            QLabel* titleLabel = new QLabel(QString("%1位置的Flyout").arg(pos.name), content);
            QFont   titleFont  = titleLabel->font();
            titleFont.setBold(true);
            titleLabel->setFont(titleFont);

            QLabel* descLabel = new QLabel("这是Flyout的内容区域，可以放置任意组件", content);
            descLabel->setWordWrap(true);

            NPushButton* closeButton = new NPushButton("关闭", content);
            closeButton->setFixedSize(80, 32);

            contentLayout->addWidget(titleLabel);
            contentLayout->addWidget(descLabel);
            contentLayout->addWidget(closeButton);

            content->setMinimumWidth(200);

            NFlyout* flyout = new NFlyout(this);
            flyout->setContent(content);
            flyout->setAttribute(Qt::WA_DeleteOnClose);

            flyout->setPlacement(pos.position);

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

            flyout->showAt(button);
        });

        positionLayout->addWidget(button);
    }
    positionLayout->addStretch();

    addSection("不同位置", positionLayout);

    QHBoxLayout* customContentLayout = new QHBoxLayout();
    customContentLayout->setSpacing(16);

    NPushButton* formFlyoutButton = new NPushButton("带表单的Flyout");
    formFlyoutButton->setFixedSize(140, 40);
    connect(formFlyoutButton, &NPushButton::clicked, this, [this, formFlyoutButton]() {
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
        submitButton->setFixedSize(80, 32);
        cancelButton->setFixedSize(80, 32);

        buttonLayout->addWidget(submitButton);
        buttonLayout->addWidget(cancelButton);

        contentLayout->addWidget(titleLabel);
        contentLayout->addLayout(formLayout);
        contentLayout->addSpacing(10);
        contentLayout->addLayout(buttonLayout);

        formContent->setMinimumWidth(300);

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

    NPushButton* listFlyoutButton = new NPushButton("带列表的Flyout");
    listFlyoutButton->setFixedSize(140, 40);

    connect(listFlyoutButton, &NPushButton::clicked, this, [this, listFlyoutButton]() {
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
        okButton->setFixedSize(80, 32);
        cancelButton->setFixedSize(80, 32);

        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);

        contentLayout->addWidget(titleLabel);
        contentLayout->addWidget(listWidget);
        contentLayout->addLayout(buttonLayout);

        listContent->setMinimumSize(250, 300);

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

    NPushButton* calendarFlyoutButton = new NPushButton("日历Flyout");
    calendarFlyoutButton->setFixedSize(140, 40);

    connect(calendarFlyoutButton, &NPushButton::clicked, this, [this, calendarFlyoutButton]() {
        QWidget*         calendarContent = new QWidget();
        QVBoxLayout*     contentLayout   = new QVBoxLayout(calendarContent);
        NCalendarWidget* calendarWidget  = new NCalendarWidget(calendarContent);
        contentLayout->addWidget(calendarWidget);

        calendarContent->setMinimumSize(250, 300);

        NFlyout* flyout = new NFlyout(this);
        flyout->setContent(calendarContent);
        flyout->setContentsMargins(0, 0, 0, 0);
        flyout->setAttribute(Qt::WA_DeleteOnClose);
        flyout->setPlacement(Qt::BottomEdge);

        flyout->showAt(calendarFlyoutButton);
    });

    customContentLayout->addWidget(formFlyoutButton);
    customContentLayout->addWidget(listFlyoutButton);
    customContentLayout->addWidget(calendarFlyoutButton);
    customContentLayout->addStretch();

    addSection("自定义内容", customContentLayout);

    QHBoxLayout* styleLayout = new QHBoxLayout();
    styleLayout->setSpacing(16);

    NPushButton* styleButton = new NPushButton("自定义样式");
    styleButton->setFixedSize(140, 40);

    connect(styleButton, &NPushButton::clicked, this, [this, styleButton]() {
        QWidget*     content       = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(content);

        QLabel* titleLabel = new QLabel("自定义样式", content);
        QFont   titleFont  = titleLabel->font();
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);

        QLabel* descLabel = new QLabel("这个Flyout使用了自定义的背景色、边框色和圆角", content);
        descLabel->setWordWrap(true);

        NPushButton* closeButton = new NPushButton("关闭", content);
        closeButton->setFixedSize(80, 32);

        contentLayout->addWidget(titleLabel);
        contentLayout->addWidget(descLabel);
        contentLayout->addWidget(closeButton);

        content->setMinimumSize(250, 150);

        NFlyout* flyout = new NFlyout(this);
        flyout->setContent(content);
        flyout->setAttribute(Qt::WA_DeleteOnClose);

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

    addSection("自定义样式", styleLayout);

    QHBoxLayout* dismissLayout = new QHBoxLayout();
    dismissLayout->setSpacing(16);

    NPushButton* onButton  = new NPushButton("启用Light Dismiss");
    NPushButton* offButton = new NPushButton("禁用Light Dismiss");
    onButton->setFixedSize(150, 40);
    offButton->setFixedSize(150, 40);

    connect(onButton, &NPushButton::clicked, this, [this, onButton]() {
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

        NFlyout* flyout = new NFlyout(this);
        flyout->setContent(content);
        flyout->setAttribute(Qt::WA_DeleteOnClose);
        flyout->setLightDismissMode(NFlyout::On);

        flyout->showAt(onButton);
    });

    connect(offButton, &NPushButton::clicked, this, [this, offButton]() {
        QWidget*     content       = new QWidget();
        QVBoxLayout* contentLayout = new QVBoxLayout(content);

        QLabel* titleLabel = new QLabel("禁用Light Dismiss", content);
        QFont   titleFont  = titleLabel->font();
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);

        QLabel* descLabel = new QLabel("点击Flyout外部区域不会关闭此Flyout，必须通过按钮关闭", content);
        descLabel->setWordWrap(true);

        NPushButton* closeButton = new NPushButton("关闭", content);
        closeButton->setFixedSize(80, 32);

        contentLayout->addWidget(titleLabel);
        contentLayout->addWidget(descLabel);
        contentLayout->addWidget(closeButton);

        content->setMinimumSize(250, 150);

        NFlyout* flyout = new NFlyout(this);
        flyout->setContent(content);
        flyout->setAttribute(Qt::WA_DeleteOnClose);

        flyout->setLightDismissMode(NFlyout::Off);

        connect(closeButton, &NPushButton::clicked, flyout, &NFlyout::hide);

        flyout->showAt(offButton);
    });

    dismissLayout->addWidget(onButton);
    dismissLayout->addWidget(offButton);
    dismissLayout->addStretch();

    addSection("Light Dismiss模式", dismissLayout);
}
