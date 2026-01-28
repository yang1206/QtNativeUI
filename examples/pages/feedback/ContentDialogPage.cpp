#include "ContentDialogPage.h"
#include <QDebug>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QtNativeUI/NContentDialog.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NPushButton.h>

ContentDialogPage::ContentDialogPage(QWidget* parent)
    : BasePage("ContentDialog 内容对话框", "标准的内容对话框，支持标题、内容和多个按钮。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NPushButton* basicDialogButton = new NPushButton("基本对话框");
    basicDialogButton->setFixedSize(140, 40);
    connect(basicDialogButton, &NPushButton::clicked, this, [this]() {
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

    NPushButton* threeButtonDialogButton = new NPushButton("三按钮对话框");
    threeButtonDialogButton->setFixedSize(140, 40);
    connect(threeButtonDialogButton, &NPushButton::clicked, this, [this]() {
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

    basicLayout->addWidget(basicDialogButton);
    basicLayout->addWidget(threeButtonDialogButton);
    basicLayout->addStretch();

    addSection("基本对话框", basicLayout);

    QHBoxLayout* customLayout = new QHBoxLayout();
    customLayout->setSpacing(16);

    NPushButton* customDialogButton = new NPushButton("自定义内容对话框");
    customDialogButton->setFixedSize(160, 40);
    connect(customDialogButton, &NPushButton::clicked, this, [this]() {
        NContentDialog* dialog = new NContentDialog(this);
        dialog->setTitle("自定义内容");

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

    customLayout->addWidget(customDialogButton);
    customLayout->addStretch();

    addSection("自定义内容", customLayout);
}
