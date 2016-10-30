#include "colouibutton.h"

ColoUiButton::ColoUiButton(QString name, ColoUiSignalManager *ss):ColoUiItem(name,ss)
{
    currentState = IS_NORMAL;
    this->type = CUI_BUTTON;
    signalInfo.type = ST_MOUSE_CLICK;
    this->setAcceptHoverEvents(true);
}

void ColoUiButton::setConfiguration(ColoUiConfiguration c){
    ColoUiItem::setConfiguration(c);
    if (!normalIcon.isNull())
        pressedIcon = normalIcon.convertToFormat(QImage::Format_Alpha8);
    update();
}

void ColoUiButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){    
    if (currentState == IS_ALTERNATIVE){
        QImage temp;
        temp = normalIcon;
        normalIcon = pressedIcon;

        ColoUiConfiguration ctemp = config;
        config.set(CPR_X,0); config.set(CPR_Y,0);
        ColoUiItem::paint(painter,option,widget);
        config = ctemp;

        normalIcon = temp;
    }
    else{
        ColoUiConfiguration ctemp = config;
        config.set(CPR_X,0); config.set(CPR_Y,0);
        ColoUiItem::paint(painter,option,widget);
        config = ctemp;
    }
}

//------------------------------ MOUSE EVENTS -----------------------------------------------------------

void ColoUiButton::mousePressEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    currentState = IS_ALTERNATIVE;
    update();
}

void ColoUiButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    currentState = IS_NORMAL;
    update();
    if (signalSender != NULL){
        signalInfo.data.clear();
        signalInfo.type = ST_MOUSE_CLICK;
        signalSender->sendSignal(signalInfo);
    }
}

void ColoUiButton::hoverEnterEvent(QGraphicsSceneHoverEvent *e){
    Q_UNUSED(e);
    currentState = IS_HOVER;
    signalInfo.data = true;
    signalInfo.type = ST_HOVER_EVENT;
    signalSender->sendSignal(signalInfo);
    update();
}

void ColoUiButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *e){
    Q_UNUSED(e);
    currentState = IS_NORMAL;
    signalInfo.data = false;
    signalInfo.type = ST_HOVER_EVENT;
    signalSender->sendSignal(signalInfo);
    update();
}


