#ifndef COLOUIVIEW_H
#define COLOUIVIEW_H

#include "colouielement.h"
#include "colouibutton.h"
#include "colouilist.h"
#include "colouitext.h"
#include <QGraphicsScene>

class ColoUiContainer;

class ColoUiView : public ColoUiBase
{

public:
    ColoUiView(QString ID, quint16 xx, quint16 yy, quint16 w, quint16 h, ColoUiTextInputDialog *diag);

    QString createElement(ColoUiElementType element,
                          QString ID,
                          ColoUiConfiguration config,
                          ColoUiSignalManager *signalManager,
                          bool dimensionsAreRelative = false);

    QRect getViewRect() const;

    ColoUiElement *element(QString name) const;

    void setViewBackgroundColor(QVariantHash c);
\
    void drawView(QGraphicsScene *scene);

    void removeView(QGraphicsScene *scene);

    void repositionElements();

    void translateView(qreal delta, bool xDelta);

    void resetDeltasAndZValue() { deltax = 0; deltay = 0; ZValue = 0;}

private:

    // To pass to the inputs
    ColoUiTextInputDialog *inputDiag;

    // Elements of View
    QMap<QString,ColoUiElement*> elements;

    // Dimensions for View
    quint16 width;
    quint16 height;
    quint16 x;
    quint16 y;

    // Used for translating;
    qreal deltax;
    qreal deltay;
    qreal ZValue;

    bool isFrontView;

    // Background Rect
    QGraphicsRectItem *background;

    // Used for collision verification
    QHash<QString,QRect> elementRects;


};

#endif // COLOUIVIEW_H
