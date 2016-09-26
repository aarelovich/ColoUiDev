#include "colouitransitionscreen.h"

ColoUiTransitionScreen::ColoUiTransitionScreen():ColoUiElement("",NULL)
{
    boundingBox = QRectF(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    transparentWindow = QRectF(0,0,10,10);
    solidColor = QColor(Qt::blue);
    this->setZValue(-1);
}

void ColoUiTransitionScreen::setConfiguration(ColoUiConfiguration c){
    Q_UNUSED(c);
}

void ColoUiTransitionScreen::setViewWindow(quint16 x, quint16 y, quint16 w, quint16 h){
    if (x+w > SCREEN_WIDTH) return;
    if (y+h > SCREEN_HEIGHT) return;

    transparentWindow = QRectF(x,y,w,h);
    update();
}

void ColoUiTransitionScreen::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);


    QPen pen;
    pen.setWidth(0);
    pen.setColor(solidColor);
    painter->setPen(pen);

    painter->setBrush(QBrush(solidColor));

    painter->drawRect(0,0,SCREEN_WIDTH,transparentWindow.top());
    painter->drawRect(0,transparentWindow.bottom(),SCREEN_WIDTH,SCREEN_HEIGHT-transparentWindow.bottom());
    painter->drawRect(0,transparentWindow.top(),transparentWindow.left(),transparentWindow.height());
    painter->drawRect(transparentWindow.right(),
                      transparentWindow.top(),
                      SCREEN_WIDTH - transparentWindow.right(),
                      transparentWindow.height());

}
