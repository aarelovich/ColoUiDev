#include "colouiview.h"

ColoUiView::ColoUiView(QString ID, quint16 xx, quint16 yy, quint16 w, quint16 h, ColoUiSignalManager *ss)
{

    elementID = ID;
    width = w;
    height = h;
    x = xx;
    y = yy;
    deltax = 0;
    deltay = 0;
    ZValue = 0;
    isFrontView = true;

    signalManager = ss;

    QPen pen;
    pen.setWidth(0);
    background = new QGraphicsRectItem(0,0,this->width,this->height);
    background->setPen(pen);
    background->setBrush(QBrush(Qt::white));

}

void ColoUiView::setViewBackgroundColor(QVariantHash c){
    QBrush b = ColoUiConfiguration::configureBrushForGradient(c,background->boundingRect());
    background->setPen(QPen(b,0));
    background->setBrush(b);
}

QString ColoUiView::createElement(ColoUiElementType element, QString ID, ColoUiConfiguration config, bool dimensionsAreRelative){

    if (dimensionsAreRelative){
        config.set(CPR_X,qMin(100.0,qreal(config.getInt32(CPR_X)))*(qreal)width/100.0);
        config.set(CPR_WIDTH,qMin(100.0,qreal(config.getInt32(CPR_WIDTH)))*(qreal)width/100.0);
        config.set(CPR_Y,qMin(100.0,qreal(config.getInt32(CPR_Y)))*(qreal)height/100.0);
        config.set(CPR_HEIGHT,qMin(100.0,qreal(config.getInt32(CPR_HEIGHT)))*(qreal)height/100.0);
    }

    if (ID.contains(".")){
        return ERROR_NAMES_CANNOT_CONTAIN_DOT;
    }

    ID = this->elementID + "." + ID;

    if (elements.contains(ID)){
        return ERROR_NAME_IN_USE;
    }

    // Checking for overlap
    QRect rect(config.getInt32(CPR_X),config.getInt32(CPR_Y),config.getUInt16(CPR_WIDTH),config.getUInt16(CPR_HEIGHT));

    QList<QString> keys = elementRects.keys();
    for (qint32 i = 0; i < keys.size(); i++){
        if (rect.intersects(elementRects.value(keys.at(i)))){
            QString error(ERROR_ELEMENT_OVERLAPS);
            error = error + "_" + keys.at(i);
            return error;
        }
    }

    // Checking for dimensions
    if (config.getInt32(CPR_X) + config.getInt32(CPR_WIDTH) > width){
        //qDebug() << "Sum" << config.getInt32(CPR_X) + config.getInt32(CPR_WIDTH) << "width" << width;
        return ERROR_ELEMENT_NOT_CONTAINED_IN_VIEW;
    }
    if (config.getInt32(CPR_Y) + config.getInt32(CPR_HEIGHT) > height){
        //qDebug() << "Sum" << config.getInt32(CPR_Y) + config.getInt32(CPR_HEIGHT) << "height" << height;
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
        coloUiElement = new ColoUiMultiLineText(ID,signalManager);
        break;
    case CUI_DROPDOWN:
        coloUiElement = new ColoUiDropdownList(ID,signalManager);
        break;
    case CUI_CHECKBOX:
        coloUiElement = new ColoUiCheckBox(ID,signalManager);
        break;
    case CUI_PROGRESS_BAR:
        coloUiElement = new ColoUiProgressBar(ID,signalManager);
        break;
    case CUI_SLIDER:
        coloUiElement = new ColoUiSlider(ID,signalManager);
        break;
    case CUI_PLACEHOLDER:
        coloUiElement = new ColoUiPlaceHolder(ID,signalManager);
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

ColoUiElement* ColoUiView::getElement(QString name) const{

    // Checking the straight up name
    if (elements.contains(name)){
        return elements.value(name);
    }
    else{
        // Cheking for the compound name
        name = this->elementID + "." + name;
        if (elements.contains(name)){
            return elements.value(name);
        }
    }
    return NULL;

}

QPoint ColoUiView::getElementPos(QString name) const{
    // MUST be compound name
    if (elementRects.contains(name)){
        QRect r = elementRects.value(name);
        return QPoint(this->x + r.left(), this->y + r.top());
    }
    else{
        return QPoint(-1,-1);
    }
}

bool ColoUiView::replacePlaceHolder(QString phID, ColoUiElement *customElement){
    if (!elements.contains(phID)) return false;
    if (elements.value(phID)->getType() != CUI_PLACEHOLDER) return false;

    ColoUiPlaceHolder * ph = (ColoUiPlaceHolder *)elements.value(phID);

    QRectF bbox = ph->boundingRect();
    customElement->forceSetDimensions(bbox.width(),bbox.height(),phID,ph->getSignalManager());

    elements[phID] = customElement;
    delete ph;
    return true;

}

QStringList ColoUiView::getElementList() const {
    return elements.keys();
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

void ColoUiView::translateView(qreal delta, bool xDelta, qreal zval){
    if (xDelta){
        deltax = deltax + delta;
    }
    else{
        deltay = deltay + delta;
    }
    ZValue = zval;
    repositionElements();
}

void ColoUiView::resetDeltasAndZValue(){
    deltax = 0;
    deltay = 0;
    ZValue = 0;
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
