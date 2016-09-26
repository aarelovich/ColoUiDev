#ifndef COLOUIITEM_H
#define COLOUIITEM_H

#include "colouielement.h"

class ColoUiItem: public ColoUiElement
{
public:
    ColoUiItem(QString name, ColoUiSignalManager * ss = 0);

    // Wrapper paint function
    void drawItem(QPainter *painter, bool alternative = false);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    void setData(QVariant data){userData = data;}
    QVariant getData() const {return userData;}

protected:
    bool isPressed;
    bool useIcon;
    QImage normalIcon;
    qreal xText;
    qreal yText;
    qreal xIcon;
    qreal yIcon;
    QVariant userData;

    // Configuration depending on Icon position.
    void configForIconAbove(QImage icon);
    void configForIconBelow(QImage icon);
    void configForIconRight(QImage icon);
    void configForIconLeft(QImage icon);


};

#endif // COLOUIITEM_H
