#include "colouiview.h"

ColoUiView::ColoUiView(QString ID, quint16 xx, quint16 yy, quint16 w, quint16 h, ColoUiTextInputDialog *diag)
{
    elementID = ID;
    width = w;
    height = h;
    x = xx;
    y = yy;
    deltax = 0;
    deltay = 0;
    ZValue = 0;
    inputDiag = diag;
    isFrontView = true;

    QPen pen;
    pen.setWidth(0);
    background = new QGraphicsRectItem(0,0,this->width,this->height);
    background->setPen(pen);
    background->setBrush(QBrush(Qt::white));
}

void ColoUiView::setViewBackgroundColor(QColor c){
    background->setBrush(QBrush(c));
}

QString ColoUiView::createElement(ColoUiElementType element, QString ID, ColoUiElementConfig config, ColoUiSignalManager *signalManager, bool dimensionsAreRelative){

    if (dimensionsAreRelative){
        config.x = qMin(100.0,qreal(config.x))*(qreal)width/100.0;
        config.width = qMin(100.0,qreal(config.width))*(qreal)width/100.0;
        config.y = qMin(100.0,qreal(config.y))*(qreal)height/100.0;
        config.height = qMin(100.0,qreal(config.height))*(qreal)height/100.0;
    }

    ID = this->elementID + "." + ID;

    if (elements.contains(ID)){
        return ERROR_NAME_IN_USE;
    }

    // Checking for overlap
    QRect rect(config.x,config.y,config.width,config.height);

    QList<QString> keys = elementRects.keys();
    for (qint32 i = 0; i < keys.size(); i++){
        if (rect.intersects(elementRects.value(keys.at(i)))){
            QString error(ERROR_ELEMENT_OVERLAPS);
            error = error + "_" + keys.at(i);
            return error;
        }
    }

    // Checking for dimensions
    if (config.x + config.width > width){
        return ERROR_ELEMENT_NOT_CONTAINED_IN_VIEW;
    }
    if (config.y + config.height > height){
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

void ColoUiView::drawView(QGraphicsScene *scene){

    scene->addItem(background);
    QList<QString> keys = elements.keys();
    for (qint32 i = 0; i < keys.size(); i++){
        ColoUiElement *element = elements.value(keys.at(i));
        scene->addItem(element);
    }
    repositionElements();

}

void ColoUiView::removeView(QGraphicsScene *scene){
    scene->removeItem(background);
    QList<QString> keys = elements.keys();
    for (qint32 i = 0; i < keys.size(); i++){
        ColoUiElement *element = elements.value(keys.at(i));
        scene->removeItem(element);
    }
}

void ColoUiView::translateView(qreal delta, bool xDelta){
    if (xDelta){
        deltax = deltax + delta;
    }
    else{
        deltay = deltay + delta;
    }
    ZValue = -2;
    repositionElements();
}

void ColoUiView::repositionElements(){

    qreal baseX = this->x + deltax;
    qreal baseY = this->y + deltay;
    background->setPos(baseX,baseY);
    background->setZValue(ZValue);

    QList<QString> keys = elements.keys();
    for (qint32 i = 0; i < keys.size(); i++){
        ColoUiElement *element = elements.value(keys.at(i));
        QRect r = elementRects.value(keys.at(i));
        qreal x = baseX + r.left();
        qreal y = baseY + r.top();
        element->setPos(x,y);
        element->setZValue(ZValue);
    }
}
