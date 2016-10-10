#include "colouiplaceholder.h"

ColoUiPlaceHolder::ColoUiPlaceHolder(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    this->type = CUI_PLACEHOLDER;
}

void ColoUiPlaceHolder::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
}

void ColoUiPlaceHolder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QBrush b = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_BACKGROUND_COLOR),boundingBox);
    painter->setBrush(b);
    painter->drawRect(boundingBox);

}
