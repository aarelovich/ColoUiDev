#include "colouielement.h"

ColoUiElement::ColoUiElement(QString name, ColoUiSignalManager *ss)
{
    elementID = name;
    signalSender = ss;
    signalInfo.elementID = elementID;
}

void ColoUiElement::setConfiguration(ColoUiElementConfig c){
    config = c;
}

QRectF ColoUiElement::boundingRect() const{
    return QRectF(0,0,100,100);
}

void ColoUiElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(QBrush(Qt::gray));
    painter->drawRect(0,0,100,100);
}
