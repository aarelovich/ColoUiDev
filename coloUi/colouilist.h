#ifndef COLOUILISTVIEW_H
#define COLOUILISTVIEW_H

#include "colouiitem.h"
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneHoverEvent>
#include <QCoreApplication>
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

    // Accessing the the item data
    ColoUiConfiguration getItemConfiguration(quint32 row, quint32 col = 0);
    bool setItemConfiguration(quint32 row, quint32 col, ColoUiConfiguration c);

    // Styling the headers.
    // If col is -1 set header config appends a header. This will create an empty item configuration for the new column if necessary
    bool setHeaderConfig(ColoUiConfiguration c, qint32 col = -1);
    ColoUiConfiguration getHeaderConfig(quint32 col);

    qint32 getRowCount() const;
    qint32 getColCount() const;


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

    // The item height;
    qreal itemH;

    qint32 hoverRow;

    bool showHeaders;

    // The data
    QVector< QVector<ColoUiConfiguration> > items;

    // The headers if enabled.
    QVector<ColoUiConfiguration> headers;

    // Where to start drawing
    void updateYStartPoint(qreal dy);

    // Getting the row and col from a mouse;
    QPoint getRowAndColForClick(QPointF mouse);

    int justSentDClick;
    void waitForAnotherClick();


};

#endif // COLOUILISTVIEW_H
