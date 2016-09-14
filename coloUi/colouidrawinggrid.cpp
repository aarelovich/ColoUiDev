#include "colouidrawinggrid.h"

ColoUiDrawingGrid::ColoUiDrawingGrid()
{
    refreshGrid();
}

void ColoUiDrawingGrid::refreshGrid(){
    boundingBox = QRectF(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
}

void ColoUiDrawingGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Drawing the background
    painter->setBrush(QBrush(Qt::gray));
    QPen pen;
    pen.setWidth(0);
    painter->setPen(pen);
    painter->drawRect(boundingBox);

    // Drawing vertical lines
    for (quint16 i = 0; i < SCREEN_WIDTH; i = i + 1){
        painter->drawLine(i,0,i,SCREEN_HEIGHT);
    }

    // Drawing horizontal lines
    for (quint16 i = 0; i < SCREEN_HEIGHT; i = i + 1){
        painter->drawLine(0,i,SCREEN_WIDTH,i);
    }


}
