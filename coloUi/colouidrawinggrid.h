#ifndef COLOUIDRAWINGGRID_H
#define COLOUIDRAWINGGRID_H

#include "colouibase.h"

class ColoUiDrawingGrid : public ColoUiBase, public QGraphicsItem
{
public:
    ColoUiDrawingGrid();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF boundingRect() const{return boundingBox;}

    void refreshGrid();

private:
    QRectF boundingBox;

};

#endif // COLOUIDRAWINGGRID_H
