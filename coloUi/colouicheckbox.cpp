#include "colouicheckbox.h"

ColoUiCheckBox::ColoUiCheckBox(QString name, ColoUiSignalManager * ss):ColoUiElement(name,ss)
{
    isChecked = false;
    this->type = CUI_CHECKBOX;
}

void ColoUiCheckBox::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);

    qreal k = (qreal)(c.getUInt16(CPR_CHECKBOX_WIDTH))/100;
    checkBox = QRectF(0,0,this->w*k,this->h);
    qreal x = checkBox.right() + c.getUInt16(CPR_X_OFFSET);
    qreal y = c.getUInt16(CPR_Y_OFFSET);
    textBox = QRectF(x,y,this->w - checkBox.width(),this->h);

    update();

}

void ColoUiCheckBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPen pen;
    pen.setWidth(config.getUInt16(CPR_BORDER_WIDTH));
    pen.setColor(config.getColor(CPR_BORDER_COLOR));
    painter->setPen(pen);

    QString g;
    if (isChecked){
        g = CPR_ALTERNATIVE_BACKGROUND_COLOR;
    }
    else{
        g = CPR_BACKGROUND_COLOR;
    }

    QBrush b = ColoUiConfiguration::configureBrushForGradient(config.getGradient(g),checkBox);
    painter->setBrush(b);

    painter->drawRect(checkBox);

    painter->setFont(config.getFont());
    pen.setColor(config.getColor(CPR_TEXT_COLOR));
    pen.setWidth(0);
    painter->setPen(pen);
    painter->drawText(textBox,config.getString(CPR_TEXT));

}


void ColoUiCheckBox::mousePressEvent(QGraphicsSceneMouseEvent *e){
    if (checkBox.contains(e->pos().x(),e->pos().y())){
        isChecked = ! isChecked;
        signalInfo.data = isChecked;
        signalInfo.type = ST_MOUSE_CLICK;
        signalSender->sendSignal(signalInfo);
        update();
    }
}
