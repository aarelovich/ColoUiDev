#include "colouiview.h"

ColoUiView::ColoUiView(QString ID)
{
    elementID = ID;
    width = 100;
    height = 100;
}

void ColoUiView::setViewSize(qreal w, qreal h){
    width = w;
    height = h;
}

void ColoUiView::addElement(ColoUiElement *element,
                            ColoUiRelativePostion rp,
                            ColoUiElementSizing es,
                            QString referenceElement){




}
