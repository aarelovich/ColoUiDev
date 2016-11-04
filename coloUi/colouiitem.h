#ifndef COLOUIITEM_H
#define COLOUIITEM_H

#include "colouielement.h"

class ColoUiItem: public ColoUiElement
{
public:

    typedef enum {IS_NORMAL, IS_ALTERNATIVE, IS_HOVER} ItemState;

    ColoUiItem(QString name, ColoUiSignalManager * ss = 0);

    // Wrapper paint function
    void drawItem(QPainter *painter, ItemState state);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    void setData(QVariant data){userData = data;}
    QVariant getData() const {return userData;}

protected:
    ItemState currentState;
    bool useIcon;
    QImage normalIcon;
    qreal xText;
    qreal yText;
    qreal xIcon;
    qreal yIcon;
    QRectF textBoundingBox;
    QRectF drawBox;
    QVariant userData;

    // Configuration depending on Icon position.
    void centerTextOnButton();
    void configForIconAbove(QImage icon);
    void configForIconBelow(QImage icon);
    void configForIconRight(QImage icon);
    void configForIconLeft(QImage icon);
    void updateDrawBox();



};

#endif // COLOUIITEM_H
