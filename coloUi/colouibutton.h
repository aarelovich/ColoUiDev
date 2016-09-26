#ifndef COLOUIBUTTON_H
#define COLOUIBUTTON_H

#include "colouiitem.h"

class ColoUiButton : public ColoUiItem
{
public:
    ColoUiButton(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    QImage pressedIcon;

};

#endif // COLOUIBUTTON_H
