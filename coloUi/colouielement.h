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
};

#endif // COLOUIELEMENT_H
