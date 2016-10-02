#ifndef COLOUITEXT_H
#define COLOUITEXT_H

#include "colouielement.h"
#include "colouitextinputdialog.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

class ColoUiText : public ColoUiElement
{
public:
    ColoUiText(QString name, ColoUiTextInputDialog *diag, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void wheelEvent(QGraphicsSceneWheelEvent *e);

private:
    ColoUiTextInputDialog *inputDialog;
    QRectF textBoundingBox;
    qreal yDisplacement;
    qreal yLastScrollPoint;

    void updateTextBoundingBox();

};

#endif // COLOUITEXT_H
