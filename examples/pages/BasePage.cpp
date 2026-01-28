#include "BasePage.h"
#include <QFrame>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NScrollArea.h>

BasePage::BasePage(const QString& title, const QString& description, QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_scrollArea = new NScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_contentWidget = new QWidget(m_scrollArea);
    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->setContentsMargins(32, 32, 32, 32);
    m_contentLayout->setSpacing(24);

    NLabel* titleLabel = new NLabel(title, NLabelType::Title, m_contentWidget);
    m_contentLayout->addWidget(titleLabel);

    if (!description.isEmpty()) {
        NLabel* descLabel = new NLabel(description, NLabelType::Body, m_contentWidget);
        m_contentLayout->addWidget(descLabel);
    }

    m_contentLayout->addStretch();

    m_scrollArea->setWidget(m_contentWidget);
    mainLayout->addWidget(m_scrollArea);
}

void BasePage::addSection(const QString& sectionTitle, QWidget* sectionWidget) {
    int count = m_contentLayout->count();
    m_contentLayout->insertWidget(count - 1, new NLabel(sectionTitle, NLabelType::Subtitle, m_contentWidget));
    m_contentLayout->insertWidget(count, sectionWidget);
}

void BasePage::addSection(const QString& sectionTitle, QLayout* sectionLayout) {
    int count = m_contentLayout->count();
    m_contentLayout->insertWidget(count - 1, new NLabel(sectionTitle, NLabelType::Subtitle, m_contentWidget));
    m_contentLayout->insertLayout(count, sectionLayout);
}
