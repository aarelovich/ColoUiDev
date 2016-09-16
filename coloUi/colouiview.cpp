#include "colouiview.h"

ColoUiView::ColoUiView(QString ID, quint16 xx, quint16 yy, quint16 w, quint16 h, ColoUiTextInputDialog *diag)
{
    elementID = ID;
    width = w;
    height = h;
    x = xx;
    y = yy;
    inputDiag = diag;
}

QString ColoUiView::createElement(ColoUiElementType element, QString ID, ColoUiElementConfig config, ColoUiSignalManager *signalManager){

    ID = this->elementID + "." + ID;

    if (elements.contains(ID)){
        return ERROR_NAME_IN_USE;
    }

    // Checking for overlap
    QRect rect(config.x,config.y,config.width,config.height);

    QHashIterator<QString,QRect>  i(elementRects);
    while (i.hasNext()){
        if (rect.intersects(i.value())){
            QString error(ERROR_ELEMENT_OVERLAPS);
            error = error + "_" + i.key();
            return error;
        }
    }

    // Checking for dimensions
    if ((config.x > width) || (config.x + config.width > width)){
        return ERROR_ELEMENT_NOT_CONTAINED_IN_VIEW;
    }
    if ((config.y > height) || (config.y + config.height > height)){
        return ERROR_ELEMENT_NOT_CONTAINED_IN_VIEW;
    }

    // Adding element to the map
    ColoUiElement *coloUiElement;

    switch (element){
    case CUI_BUTTON:
        coloUiElement = new ColoUiButton(ID,signalManager);
        break;
    case CUI_LIST:
        coloUiElement = new ColoUiList(ID,signalManager);
        break;
    case CUI_TEXT:
        coloUiElement = new ColoUiText(ID,inputDiag,signalManager);
        break;
    default:
        return ERROR_UNKNOWN_ELEMENT_TYPE;
    }

    elements[ID] = coloUiElement;
    elementRects[ID] = rect;
    coloUiElement->setConfiguration(config);

    return "";

}

QRect ColoUiView::getViewRect() const{
    return QRect(x,y,width,height);
}

void ColoUiView::drawView(QGraphicsScene *scene, qreal scaleFactor){

    QList<QString> keys = elements.keys();
    for (qint32 i = 0; i < keys.size(); i++){
        ColoUiElement *element = elements.value(keys.at(i));
        QRect r = elementRects.value(keys.at(i));
        scene->addItem(element);
        element->setPos(this->x + r.left(),this->y + r.top());
        element->setZValue(10);
        element->setScale(scaleFactor);
    }

}
