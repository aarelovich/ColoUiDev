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

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    bool isPressed;
    bool useIcon;
    QImage normalIcon;
    QImage pressedIcon;
    qreal xText;
    qreal yText;
    qreal xIcon;
    qreal yIcon;

    // Configuration depending on Icon position.
    void configForIconAbove(QImage icon);
    void configForIconBelow(QImage icon);
    void configForIconRight(QImage icon);
    void configForIconLeft(QImage icon);

};

#endif // COLOUIBUTTON_H
