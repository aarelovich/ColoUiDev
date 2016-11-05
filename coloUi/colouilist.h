#ifndef COLOUILISTVIEW_H
#define COLOUILISTVIEW_H

#include "colouiitem.h"
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneHoverEvent>
#include <QCoreApplication>
#include <QCursor>
#include <QTime>


class ColoUiList : public ColoUiElement
{
public:
    ColoUiList(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    // If where is -1 the row will be inserted at the bottom of the table.
    void insertRow(qint32 where = -1);

    // Delete row (Can't delete just one element)
    void deleteRow(qint32 where);

    // Adjusts displacemente variables so the the row is visible.
    void scrollToRow(qint32 row);

    // Clear the data
    void clearData();

    // Accessing the the item data
    ColoUiConfiguration getItemConfiguration(quint32 row, quint32 col = 0);
    bool setItemConfiguration(quint32 row, quint32 col, ColoUiConfiguration c);

    // Styling the headers.
    // If col is -1 set header config appends a header. This will create an empty item configuration for the new column if necessary
    bool setHeaderConfig(ColoUiConfiguration c, qint32 col = -1);
    ColoUiConfiguration getHeaderConfig(quint32 col);

    qint32 getRowCount() const;
    qint32 getColCount() const;

    void setLockColumnWidths(bool enable) {lockColWidths = enable;}
    bool columnWidthsLocked() const {return lockColWidths;}


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void wheelEvent(QGraphicsSceneWheelEvent *e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);

private:
    // Scrolling variables
    qreal yStartPoint;
    qreal yLastScrollPoint;
    qreal maxYDisplacement;

    // Scrollbar variables
    qreal scrollBarWidth;
    qreal endScrollBarPoint;
    qreal sliderHeight;
    qreal sliderPosition;
    qreal scrollBarX;
    bool movingSlider;
    bool scrollEnabled;

    // The item height;
    qreal itemH;

    // Which row to highlight
    qint32 hoverRow;

    // Define wheather to show headers or not
    bool showHeaders;

    // Which columns to resize
    QVector<qint32> resizeColumns;
    qreal xLastColoWidthPoint;
    static const qint32 MIN_COL_WIDTH = 10; // No column can be less than 10 units

    // The data
    QVector< QVector< ColoUiItem* > > items;

    // The headers if enabled.
    QVector<ColoUiConfiguration> headers;

    // Where to start drawing
    void updateYStartPoint(qreal dy);

    // Getting the row and col from a mouse;
    QPoint getRowAndColForClick(QPointF mouse, qreal *diffx = nullptr);

    int justSentDClick;
    void waitForAnotherClick();

    // Function that draws the headers
    void drawHeaders(QPainter *painter);
    void drawScrollBar(QPainter *painter);

    // To see if columns should be dimensioned
    bool lockColWidths;

    // Function that updates an items dimensions only when necessary.
    void redimensionItemWidths();


};

#endif // COLOUILISTVIEW_H
