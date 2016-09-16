#ifndef COLOUIBUTTON_H
#define COLOUIBUTTON_H

#include "colouielement.h"

class ColoUiButton : public ColoUiElement
{
public:
    ColoUiButton(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiElementConfig c);

private:
    bool isPressed;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
};

#endif // COLOUIBUTTON_H
