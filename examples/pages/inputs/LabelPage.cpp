#include "LabelPage.h"
#include <QFrame>
#include <QGridLayout>
#include <QtNativeUI/NLabel.h>

struct LabelStyleInfo {
    QString          exampleText;
    NLabelType::Type labelType;
    QString          fontDescription;
    QString          sizeDescription;
};

LabelPage::LabelPage(QWidget* parent)
    : BasePage("Label 标签", "Label 用于显示文本内容，支持多种预设样式。", parent) {

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setHorizontalSpacing(24);
    gridLayout->setVerticalSpacing(0);

    NLabel* exampleHeader = new NLabel("示例", NLabelType::BodyStrong, this);
    NLabel* fontHeader    = new NLabel("字体", NLabelType::BodyStrong, this);
    NLabel* sizeHeader    = new NLabel("大小/行高", NLabelType::BodyStrong, this);
    gridLayout->addWidget(exampleHeader, 0, 0);
    gridLayout->addWidget(fontHeader, 0, 1);
    gridLayout->addWidget(sizeHeader, 0, 2);

    QList<LabelStyleInfo> styles = {{"Caption", NLabelType::Caption, "Small, Regular", "12/16 epx"},
                                    {"Body", NLabelType::Body, "Text, Regular", "14/20 epx"},
                                    {"Body Strong", NLabelType::BodyStrong, "Text, SemiBold", "14/20 epx"},
                                    {"Body Large", NLabelType::BodyLarge, "Text, Regular", "18/20 epx"},
                                    {"Body Strong Large", NLabelType::BodyStrongLarge, "Text, SemiBold", "18/20 epx"},
                                    {"Subtitle", NLabelType::Subtitle, "Display, SemiBold", "20/28 epx"},
                                    {"Title", NLabelType::Title, "Display, SemiBold", "28/36 epx"},
                                    {"Title Large", NLabelType::TitleLarge, "Display, SemiBold", "40/52 epx"},
                                    {"Display", NLabelType::Display, "Display, SemiBold", "68/92 epx"}};

    int row = 1;
    for (const auto& style : styles) {
        NLabel* exampleLabel = new NLabel(style.exampleText, style.labelType, this);
        NLabel* fontLabel    = new NLabel(style.fontDescription, NLabelType::Caption, this);
        NLabel* sizeLabel    = new NLabel(style.sizeDescription, NLabelType::Caption, this);

        gridLayout->addWidget(exampleLabel, row, 0);
        gridLayout->addWidget(fontLabel, row, 1);
        gridLayout->addWidget(sizeLabel, row, 2);

        QFrame* separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Plain);
        separator->setStyleSheet("background-color: #f0f0f0;");
        separator->setFixedHeight(1);
        gridLayout->addWidget(separator, row + 1, 0, 1, 3);

        row += 2;
    }

    gridLayout->setColumnStretch(0, 2);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 1);

    for (int i = 0; i <= row; i += 2) {
        gridLayout->setRowMinimumHeight(i, 48);
    }

    addSection("样式列表", gridLayout);
}
