#include "colouibutton.h"

ColoUiButton::ColoUiButton(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    isPressed = false;
    signalInfo.type = ST_MOUSE_CLICK;
}

void ColoUiButton::setConfiguration(ColoUiElementConfig c){
    ColoUiElement::setConfiguration(c);
    update();
}

void ColoUiButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    Q_UNUSED(widget)
    Q_UNUSED(option)
    drawItem(painter,isPressed);
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


