#ifndef COLOUILISTVIEW_H
#define COLOUILISTVIEW_H

#include "colouielement.h"


class ColoUiList : public ColoUiElement
{
public:
    ColoUiList(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const {return boundingBox;}
    void setConfiguration(ColoUiElementConfig c);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);


private:
    QRectF boundingBox;

    // Scrolling variables
    qreal yStartPoint;
    qreal lastYpoint;
    qreal maxYDisplacement;

};

#endif // COLOUILISTVIEW_H
