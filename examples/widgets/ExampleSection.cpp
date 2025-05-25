#include "ExampleSection.h"
#include <QLabel>
#include <QVBoxLayout>

ExampleSection::ExampleSection(const QString& title, QWidget* content, QWidget* parent) : QWidget(parent) {
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(16);

    // 创建标题
    m_titleLabel    = new QLabel(title, this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_layout->addWidget(m_titleLabel);

    // 创建内容区域
    QWidget* contentWidget = new QWidget(this);
    m_contentLayout        = new QVBoxLayout(contentWidget);
    m_contentLayout->setSpacing(16);
    m_contentLayout->setContentsMargins(0, 0, 0, 0);

    if (content) {
        m_contentLayout->addWidget(content);
    }

    m_layout->addWidget(contentWidget);
}

void ExampleSection::setContent(QWidget* content) {
    // 清除现有内容
    QLayoutItem* child;
    while ((child = m_contentLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    if (content) {
        m_contentLayout->addWidget(content);
    }
}

QVBoxLayout* ExampleSection::contentLayout() const { return m_contentLayout; }