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
        boundingBox = QRectF(0,0,w,h);
    }
}

void ColoUiElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(QBrush(Qt::gray));
    painter->drawRect(0,0,100,100);
}


void ColoUiElement::drawItem(QPainter *painter, bool alternativeColors){

    qDebug() << "Alternative " << alternativeColors;

    QFontMetrics metrics(config.font);
    QRectF brect = metrics.boundingRect(config.text);

    // Painter set up to draw the shape
    QPen pen;
    QBrush brush;
    pen.setWidth(config.borderWidth);
    pen.setColor(config.borderColor);

    if (alternativeColors){
        painter->setBrush(QBrush(config.alternativeBackgroundColor));
    }
    else{
        painter->setBrush(QBrush(config.backgroundColor));
    }

    painter->setPen(pen);

    switch (config.shape){
    case SHAPE_ELLIPSE:
        painter->drawEllipse(boundingBox);
        break;
    case SHAPE_ROUNDED_RECT:
        painter->drawRoundedRect(boundingBox,config.roundeRectRadious,config.roundeRectRadious);
        break;
    default:
        painter->drawRect(boundingBox);
        break;
    }

//    if (boundingBox.contains(brect)){
//        pen.setColor(config.textColor);
//        pen.setWidth(0);
//        brush.setColor(config.textColor);
//        pen.setBrush(brush);
//        painter->drawText(boundingBox,Qt::AlignCenter,config.text);
//    }

    pen.setColor(config.textColor);
    pen.setWidth(0);
    brush.setColor(config.textColor);
    pen.setBrush(brush);
    painter->drawText(boundingBox,Qt::AlignCenter,config.text);

}
