#ifndef COLOUIVIEW_H
#define COLOUIVIEW_H

#include "colouielement.h"
#include "colouibutton.h"
#include "colouilist.h"
#include "colouitext.h"
#include <QGraphicsScene>

class ColoUiView : public ColoUiBase
{
public:
    ColoUiView(QString ID, quint16 xx, quint16 yy, quint16 w, quint16 h, ColoUiTextInputDialog *diag);

    QString createElement(ColoUiElementType element, QString ID, ColoUiElementConfig config);

    QRect getViewRect() const;

    void drawView(QGraphicsScene *scene);

private:

    // View Signal's emitter
    ColoUiSignalManager *signalManager;

    // To pass to the inputs
    ColoUiTextInputDialog *inputDiag;

    // Elements of View
    QHash<QString,ColoUiElement*> elements;

    // Dimensions for View
    quint16 width;
    quint16 height;
    quint16 x;
    quint16 y;

    // Used for collision verification
    QHash<QString,QRect> elementRects;


};

#endif // COLOUIVIEW_H
