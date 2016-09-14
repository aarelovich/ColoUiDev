#include "colouielement.h"

ColoUiElement::ColoUiElement(QString name, ColoUiSignalManager *ss)
{
    elementID = name;
    signalSender = ss;
    signalInfo.elementID = elementID;
    w = 0; h =0;
}

void ColoUiElement::setConfiguration(ColoUiElementConfig c){    
    config = c;
    // These are only set when configuration is called for the first time
    if ((w == 0) && (h == 0)){
        w = c.width;
        h = c.height;
    }
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


void ColoUiElement::drawItem(QPainter *painter, bool colorsLighter){

    QFontMetrics metrics(config.font);
    //QRectF brect = metrics.boundingRect(config.text);
    painter->setBrush(QBrush(config.backgroundColor));

    switch (config.shape){
    case SHAPE_ELLIPSE:
        //painter->drawEllipse();
        break;
    case SHAPE_ROUNDED_RECT:
        break;
    default:
        break;
    }

}
