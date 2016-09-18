#ifndef COLOUITRANSITIONSCREEN_H
#define COLOUITRANSITIONSCREEN_H

#include "colouielement.h"

class ColoUiTransitionScreen : public ColoUiElement
{
public:
    ColoUiTransitionScreen();

    //
    void setViewWindow(quint16 x, quint16 y, quint16 w, quint16 h);
    void setShowColor(QColor c){solidColor = c;}

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiElementConfig c);

private:
    QRectF transparentWindow;
    QColor solidColor;

};

#endif // COLOUITRANSITIONSCREEN_H
