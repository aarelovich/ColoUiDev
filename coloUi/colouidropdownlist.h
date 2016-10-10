#ifndef COLOUIDROPDOWNLIST_H
#define COLOUIDROPDOWNLIST_H

#include "colouiitem.h"

class ColoUiDropdownList: public ColoUiElement
{
public:
    ColoUiDropdownList(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    ColoUiConfiguration getItem(qint32 id) const;
    void addItem(ColoUiConfiguration id);

    // Adds an item by using the last configuration added as a template.
    // If configuration is empty it creates a standard configuration.
    void addItem(QString text);

    ColoUiConfiguration getCurrentItem() const;

    void clearSelection(); // This resets to the non selection simple text

    // Eliminates all items
    void clearItems();

    // Remove a single item
    void removeItem(qint32 index);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    void wheelEvent(QGraphicsSceneWheelEvent *e);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);

private:

    // If it is dropped or not
    bool dropped;
    bool droppedJustChanged;

    // The button part and each of the items configuration
    ColoUiConfiguration main;
    QVector<ColoUiConfiguration>  items;

    // Currently selected item and where to start drawing.
    qint32 currentIndex;
    qint32 itemToStartDrawing;
    qint32 hoverItem;

    // Arrow drawing variables.
    qreal directionIndicatorHeight;
    QRectF centerArrow;
    QRectF arrowLeft;
    QRectF arrowRight;
    QPainterPath dropDownIndicator;

    // Scrolling variables
    qreal lastY;
    qreal accDeltaY;

    void updateItemToDraw(bool up);

    typedef enum {TT_UP, TT_DOWN, TT_AT_45} TriangleType;
    QPainterPath drawArrow(QRectF r, TriangleType tt);

    void ply();

};

#endif // COLOUIDROPDOWNLIST_H
