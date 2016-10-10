#ifndef COLOUICHECKBOX_H
#define COLOUICHECKBOX_H

#include "colouielement.h"

class ColoUiCheckBox: public ColoUiElement
{
public:
    ColoUiCheckBox(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    bool getChecked() const {return isChecked;}
    void setChecked(bool c) {isChecked = c; update();}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);

private:

    bool isChecked;
    QRectF textBox;
    QRectF checkBox;
};

#endif // COLOUICHECKBOX_H
