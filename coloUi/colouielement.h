#ifndef COLOUIELEMENT_H
#define COLOUIELEMENT_H

#include "colouisignalmanager.h"
#include "colouibase.h"

class ColoUiElement: public ColoUiBase, public QGraphicsItem
{
public:
    ColoUiElement(QString name, ColoUiSignalManager * ss = 0);

    ColoUiElementConfig getConfiguration() const {return config;}

    ColoUiElementType getType() const {return type;}

    virtual void setConfiguration(ColoUiElementConfig c);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual QRectF boundingRect() const;


protected:

    ColoUiElementConfig config;
    ColoUiSignalManager *signalSender;
    ColoUiSignalEventInfo signalInfo;
    ColoUiElementType type;

    // Main properties that are only set once in the configuration and then never again used
    quint16 w;
    quint16 h;

    // Draws an item. An item is a shape with a color, centered text of a certain format and possibly and icon.
    void drawItem(QPainter *painter, bool colorsLighter = false);
};

#endif // COLOUIELEMENT_H
