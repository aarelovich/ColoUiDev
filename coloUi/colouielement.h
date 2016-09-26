#ifndef COLOUIELEMENT_H
#define COLOUIELEMENT_H

#include "colouisignalmanager.h"
#include "colouibase.h"
#include "colouiconfiguration.h"

class ColoUiElement: public ColoUiBase, public QGraphicsItem
{
public:
    ColoUiElement(QString name, ColoUiSignalManager * ss = 0);

    ColoUiConfiguration getConfiguration() const {return config;}

    ColoUiElementType getType() const {return type;}

    virtual void setConfiguration(ColoUiConfiguration c);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF boundingRect() const {return boundingBox;}


protected:

    QRectF boundingBox;

    ColoUiConfiguration config;
    ColoUiSignalManager *signalSender;
    ColoUiSignalEventInfo signalInfo;
    ColoUiElementType type;

    // Main properties that are only set once in the configuration and then never again used
    quint16 w;
    quint16 h;

};

#endif // COLOUIELEMENT_H
