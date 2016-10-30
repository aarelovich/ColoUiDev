#include "colouislider.h"

ColoUiSlider::ColoUiSlider(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    currentValue = 0;
    this->type = CUI_SLIDER;
}

void ColoUiSlider::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
    isVertical = (this->w < this->h);

    qreal sliderH;
    qreal sliderW;
    if (isVertical){
        sliderH = (qreal)c.getUInt16(CPR_SLIDER_SPREAD)*this->h/100.0;
        sliderW =  this->w;
        maxSliderPos = this->h - sliderH;
    }
    else{
        sliderW = (qreal)c.getUInt16(CPR_SLIDER_SPREAD)*this->w/100.0;
        sliderH =  this->h;
        maxSliderPos = this->w - sliderW;
    }
    slider = QRectF(0,0,sliderW,sliderH);
    isSliding = false;
    sliderPos = 0;

}


void ColoUiSlider::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPen pen;
    pen.setWidth(config.getUInt16(CPR_BORDER_WIDTH));
    pen.setColor(QColor(config.getColor(CPR_BORDER_COLOR)));
    QBrush b = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_BACKGROUND_COLOR),boundingBox);
    painter->setPen(pen);
    painter->setBrush(b);
    painter->drawRect(boundingBox);

    // Drawing the slider
    pen.setWidth(0);
    painter->setPen(pen);
    if (isVertical){
        slider.moveTop(sliderPos);
    }
    else{
        slider.moveLeft(sliderPos);
    }

    b = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_SCROLL_SLIDER),slider);
    painter->setBrush(b);
    painter->drawRect(slider);


}

void ColoUiSlider::mousePressEvent(QGraphicsSceneMouseEvent *e){
    if (slider.contains(e->pos())){
        isSliding = true;
        if (isVertical){
            lastPos = e->pos().y();
        }
        else{
            lastPos = e->pos().x();
        }
    }
}

void ColoUiSlider::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
    if (isSliding){
        qreal delta;
        if (isVertical){
            delta = e->pos().y() - lastPos;
            lastPos = e->pos().y();
        }
        else{
            delta = e->pos().x() - lastPos;
            lastPos = e->pos().x();
        }

        sliderPos = sliderPos + delta;
        if (sliderPos > maxSliderPos) sliderPos = maxSliderPos;
        else if (sliderPos < 0) sliderPos = 0;

        qint32 value = sliderPos*100.0/maxSliderPos;
        if (value != currentValue){
            signalInfo.type = ST_VALUE_CHANGED;
            signalInfo.data = value;
            signalSender->sendSignal(signalInfo);
        }
        currentValue = value;

        update();
    }
}

void ColoUiSlider::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e);
    isSliding = false;
    signalInfo.type = ST_SLIDING_STOPPED;
    signalSender->sendSignal(signalInfo);
}

void ColoUiSlider::setCurrentValue(qreal v){
    if (v < 0) v = 0;
    else if (v > 100) v = 100;
    currentValue = v;
    sliderPos = v*maxSliderPos/100.0;
    update();
}
