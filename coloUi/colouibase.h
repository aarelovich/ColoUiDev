#ifndef COLOUIBASE_H
#define COLOUIBASE_H

#include "colouidefines.h"

class ColoUiBase
{
public:
    ColoUiBase();
    QString getElementID() const {return elementID;}

protected:

    static qreal SCREEN_WIDTH;
    static qreal SCREEN_HEIGHT;

    QString elementID;
};

#endif // COLOUIBASE_H
