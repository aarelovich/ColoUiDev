#include "colouibutton.h"

ColoUiButton::ColoUiButton(QString name, ColoUiSignalManager *ss):ColoUiItem(name,ss)
{
    isPressed = false;
    this->type = CUI_BUTTON;
    signalInfo.type = ST_MOUSE_CLICK;
}

void ColoUiButton::setConfiguration(ColoUiConfiguration c){
    ColoUiItem::setConfiguration(c);
    if (!normalIcon.isNull())
        pressedIcon = normalIcon.convertToFormat(QImage::Format_Alpha8);
    update();
}

void ColoUiButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){    
    if (isPressed){
        QImage temp;
        temp = normalIcon;
        normalIcon = pressedIcon;
        ColoUiItem::paint(painter,option,widget);
        normalIcon = temp;
    }
    else{
        ColoUiItem::paint(painter,option,widget);
    }
}

//------------------------------ MOUSE EVENTS -----------------------------------------------------------

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


