#include "colouibutton.h"

ColoUiButton::ColoUiButton(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{

    config.backgroundColor = QColor(Qt::gray);
    config.text = "Button";
    config.textColor = QColor(Qt::black);
    config.height = 100;
    config.width = 200;
    boundingBox = QRectF(0,0,config.width,config.height);
    isPressed = false;

    signalInfo.type = ST_MOUSE_CLICK;
}

void ColoUiButton::setConfiguration(ColoUiElementConfig c){
    config = c;
    update();
}

void ColoUiButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    Q_UNUSED(widget)
    Q_UNUSED(option)
    if (isPressed){
        painter->setBrush(QBrush(config.backgroundColor.lighter()));
        painter->drawRect(boundingBox);
        painter->setBrush(QBrush(config.textColor.lighter()));
        painter->drawText(boundingBox,config.text);
    }
    else{
        painter->setBrush(QBrush(config.backgroundColor));
        painter->drawRect(boundingBox);
        painter->setBrush(QBrush(config.textColor));
        painter->drawText(boundingBox,config.text);
    }

}

void ColoUiButton::mousePressEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    isPressed = true;
    update();
}

void ColoUiButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    isPressed = false;
    update();
    if (signalSender != NULL){
        signalSender->sendSignal(signalInfo);
    }
}


