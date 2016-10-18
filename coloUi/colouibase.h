#ifndef COLOUIBASE_H
#define COLOUIBASE_H

#include "colouidefines.h"

class ColoUiBase
{
public:
    ColoUiBase();
    QString getElementID() const {return elementID;}

protected:

    static quint16 SCREEN_WIDTH;
    static quint16 SCREEN_HEIGHT;
    static quint16 SOFTKEYBOARD_HEIGHT;
    static bool virtualKeyboardInUse;

    QString elementID;
};

#endif // COLOUIBASE_H
