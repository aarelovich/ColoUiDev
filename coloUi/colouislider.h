#ifndef COLOUISLIDER_H
#define COLOUISLIDER_H

#include "colouielement.h"

class ColoUiSlider : public ColoUiElement
{
public:
    ColoUiSlider(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    qint32 getCurrentValue() const {return currentValue;}
    void setCurrentValue(qreal v);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
private:

    // Defines if this is a vertical slider or a horizontal one
    bool isVertical;

    // Slider to draw
    QRectF slider;

    // The int value
    qint32 currentValue;

    // Slider postion variables
    bool isSliding;
    qreal sliderPos;
    qreal maxSliderPos;
    qreal lastPos;


};

#endif // COLOUISLIDER_H
