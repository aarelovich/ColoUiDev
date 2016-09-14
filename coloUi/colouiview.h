#ifndef COLOUIVIEW_H
#define COLOUIVIEW_H

#include "colouielement.h"

class ColoUiView : public ColoUiBase
{
public:
    ColoUiView(QString ID);
    void setViewSize(qreal w, qreal h);

    void addElement(ColoUiElement *element,
                    ColoUiRelativePostion rp = RP_LEFT,
                    ColoUiElementSizing es = ES_FIT_IN_WIDTH,
                    QString referenceElement = "");

private:

    QHash<QString,ColoUiElement*> elements;
    qreal width;
    qreal height;
};

#endif // COLOUIVIEW_H
