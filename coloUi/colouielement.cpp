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

QKeyEvent * ColoUiElement::createKeyEvent(ColoUiKeyType kt, QString data){
    QKeyEvent *e = nullptr;
    switch (kt){
    case KT_BACKSPACE:
        e = new QKeyEvent(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier);
        break;
    case KT_COPY:
        e = new QKeyEvent(QEvent::KeyPress,Qt::Key_C,Qt::ControlModifier);
        break;
    case KT_ENTER:
        e = new QKeyEvent(QEvent::KeyPress,Qt::Key_Return,Qt::NoModifier);
        break;
    case KT_PASTE:
        e = new QKeyEvent(QEvent::KeyPress,Qt::Key_V,Qt::ControlModifier);
        break;
    case KT_TEXT:
        if (data == " ") e = new QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier,data);
        else e = new QKeyEvent(QEvent::KeyPress,Qt::Key_A,Qt::NoModifier,data);
        break;
    case KT_UP:
        e = new QKeyEvent(QEvent::KeyPress,Qt::Key_Up,Qt::NoModifier);
        break;
    case KT_DOWN:
        e = new QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier);
        break;
    case KT_LEFT:
        e = new QKeyEvent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier);
        break;
    case KT_RIGHT:
        e = new QKeyEvent(QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier);
        break;
    case KT_DEL:
        e = new QKeyEvent(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier);
        break;
    case KT_HIDE:
        virtualKeyboardInUse = false;
        break;
    default:
        break;
    }

    return e;
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

