#ifndef COLOUITEXT_H
#define COLOUITEXT_H

#include "colouielement.h"
#include "colouitextinputdialog.h"

class ColoUiText : public ColoUiElement
{
public:
    ColoUiText(QString name, ColoUiTextInputDialog *diag, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const {return boundingBox;}
    void setConfiguration(ColoUiElementConfig c);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QRectF boundingBox;
    ColoUiTextInputDialog *inputDialog;

};

#endif // COLOUITEXT_H
