#ifndef COLOUILISTVIEW_H
#define COLOUILISTVIEW_H

#include "colouielement.h"


class ColoUiList : public ColoUiElement
{
public:
    ColoUiList(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiElementConfig c);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);


private:
    // Scrolling variables
    qreal yStartPoint;
    qreal lastYpoint;
    qreal maxYDisplacement;

};

#endif // COLOUILISTVIEW_H
