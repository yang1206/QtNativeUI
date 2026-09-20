#ifndef QTNATIVEUI_NTABLEVIEW_H
#define QTNATIVEUI_NTABLEVIEW_H

#include <QTableView>

#include "NColor.h"
#include "stdafx.h"

class NTableViewPrivate;
class QTNATIVEUI_EXPORT NTableView : public QTableView {
    Q_OBJECT
    Q_Q_CREATE(NTableView)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHeaderBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHeaderBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHeaderHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHeaderHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHeaderPressedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHeaderPressedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemSelectedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemSelectedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHeaderTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHeaderTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPlaceholderTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPlaceholderTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDividerColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDividerColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(int, RowHeight)
    Q_PROPERTY_CREATE_Q_H(int, HeaderHeight)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, CellHorizontalPadding)
    Q_PROPERTY_CREATE_Q_H(QString, PlaceholderText)
    Q_PROPERTY_CREATE_Q_H(bool, BorderVisible)
    Q_PROPERTY_CREATE_Q_H(bool, BackgroundVisible)

  public:
    explicit NTableView(QWidget* parent = nullptr);
    ~NTableView() override;

    void setModel(QAbstractItemModel* model) override;
    void setSelectionModel(QItemSelectionModel* selectionModel) override;

    bool isShowingPlaceholder() const;

    void setVerticalHeaderVisible(bool visible);
    bool isVerticalHeaderVisible() const;

  protected:
    void keyPressEvent(QKeyEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void scrollContentsBy(int dx, int dy) override;

  private:
    friend class NTableViewPrivate;
    void init();
    void applyThemePalette();
    void connectModelSignals(QAbstractItemModel* model);
    void refreshModelPresentation();
    void syncOverlayScrollBars();
    void setHoveredRow(int row);
    void updateAutomaticAccessibleDescription();
    void updateRoundedMask();
};

#endif
