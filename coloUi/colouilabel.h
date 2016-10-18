#ifndef COLOUILABEL_H
#define COLOUILABEL_H

#include "colouielement.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

class ColoUiLabel : public ColoUiElement
{
public:
    ColoUiLabel(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

};

#endif // COLOUILABEL_H
