#ifndef EXAMPLESECTION_H
#define EXAMPLESECTION_H

#include <QWidget>
#include <QString>

class QLabel;
class QVBoxLayout;

class ExampleSection : public QWidget {
    Q_OBJECT
public:
    explicit ExampleSection(const QString& title, QWidget* content = nullptr, QWidget* parent = nullptr);
    
    void setContent(QWidget* content);
    QVBoxLayout* contentLayout() const;

private:
    QLabel* m_titleLabel;
    QVBoxLayout* m_layout;
    QVBoxLayout* m_contentLayout;
};

#endif // EXAMPLESECTION_H