#ifndef COLOUIVIEW_H
#define COLOUIVIEW_H

#include "colouielement.h"
#include "colouibutton.h"
#include "colouilist.h"
#include "colouimultilinetext.h"
#include "colouidropdownlist.h"
#include "colouicheckbox.h"
#include "colouiprogressbar.h"
#include "colouislider.h"
#include "colouiplaceholder.h"
#include "colouilineedit.h"
#include <QGraphicsScene>

class ColoUiContainer;

class ColoUiView : public ColoUiBase
{

public:
    ColoUiView(QString ID, quint16 xx, quint16 yy, quint16 w, quint16 h, ColoUiSignalManager *ss);

    QString createElement(ColoUiElementType getElement,
                          QString ID,
                          ColoUiConfiguration config,
                          bool dimensionsAreRelative = false);

    QRect getViewRect() const;

    ColoUiElement *getElement(QString name) const;

    QPoint getElementPos(QString name) const;

    bool replacePlaceHolder(QString phID, ColoUiElement *customElement);

    QStringList getElementList() const;

    QString currentSizeStructure() const;

    void setViewBackgroundColor(QVariantHash c);
\
    void drawView(QGraphicsScene *scene);

    void removeView(QGraphicsScene *scene);

    void repositionElements();

    void translateView(qreal delta, bool xDelta, qreal zval = -2);

    void resetDeltasAndZValue();

    QPoint getPos() const {return QPoint(x,y);}

private:

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

    // Copy of the SignalManager
    ColoUiSignalManager *signalManager;

    // Background Rect
    QGraphicsRectItem *background;

    // Used for collision verification
    QHash<QString,QRect> elementRects;


};

#endif // COLOUIVIEW_H
