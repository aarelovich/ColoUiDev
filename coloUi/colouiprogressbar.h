#ifndef COLOUIPROGRESSBAR_H
#define COLOUIPROGRESSBAR_H

#include "colouielement.h"

class ColoUiProgressBar : public ColoUiElement
{
public:
    ColoUiProgressBar(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    void setPercent(qreal p);

private:
    qreal percent;

};

#endif // COLOUIPROGRESSBAR_H
