#include "colouielement.h"

ColoUiElement::ColoUiElement(QString name, ColoUiSignalManager *ss)
{
    elementID = name;
    signalSender = ss;
    signalInfo.elementID = elementID;
    w = 0; h =0;
}

void ColoUiElement::forceSetDimensions(quint16 ww, quint16 hh, QString id, ColoUiSignalManager *ss){
    w = ww;
    h = hh;
    boundingBox = QRectF(0,0,ww,hh);
    config.set(CPR_WIDTH,ww);
    config.set(CPR_HEIGHT,hh);
    elementID = id;
    signalInfo.elementID = id;
    signalSender = ss;
}

void ColoUiElement::setConfiguration(ColoUiConfiguration c){    
    config = c;
    // These are only set when configuration is called for the first time
    if ((w == 0) && (h == 0)){
        w = c.getUInt16(CPR_WIDTH);
        h = c.getUInt16(CPR_HEIGHT);
        boundingBox = QRectF(0,0,w,h);
    }
}

void ColoUiElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(QBrush(Qt::gray));
    painter->drawRect(0,0,100,100);
}

