#include "colouiprogressbar.h"

ColoUiProgressBar::ColoUiProgressBar(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    percent = 0;
    this->type = CUI_PROGRESS_BAR;
}


void ColoUiProgressBar::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
}


void ColoUiProgressBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen;
    pen.setWidth(config.getUInt16(CPR_BORDER_WIDTH));
    pen.setColor(QColor(config.getColor(CPR_BORDER_COLOR)));

    // Drawing background bar
    QBrush b = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_BACKGROUND_COLOR),boundingBox);
    painter->setPen(pen);
    painter->setBrush(b);
    painter->drawRect(boundingBox);

    // Drawing foreground bar
    pen.setWidth(0);
    pen.setColor(config.getColor(CPR_TEXT_COLOR));
    b = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_ALTERNATIVE_BACKGROUND_COLOR),boundingBox);
    painter->setPen(pen);
    painter->setBrush(b);
    qreal per_width = percent*this->w/100.0;
    painter->drawRect(QRectF(0,0,per_width,this->h));

    // Drawing text if enabled.
    if (config.getBool(CPR_SHOW_VALUE)){
        qint32 p = percent;
        painter->setFont(config.getFont());
        painter->drawText(boundingBox,Qt::AlignCenter,QString::number(p) + "%");
    }

}

void ColoUiProgressBar::setPercent(qreal p){
    percent = p;
    if (percent > 100) percent = 100;
    else if (percent < 0) percent = 0;
    update();
}
