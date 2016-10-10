#ifndef COLOUIPLACEHOLDER_H
#define COLOUIPLACEHOLDER_H

#include "colouielement.h"

class ColoUiPlaceHolder : public ColoUiElement
{
public:
    ColoUiPlaceHolder(QString name, ColoUiSignalManager *ss);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    ColoUiSignalManager * getSignalManager() const {return signalSender;}

};

#endif // COLOUIPLACEHOLDER_H
