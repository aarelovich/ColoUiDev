#include "colouidropdownlist.h"

ColoUiDropdownList::ColoUiDropdownList(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    this->type = CUI_DROPDOWN;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    this->setAcceptHoverEvents(true);
    plyList = new PlyList(this);
    toggleList(false);
}


void ColoUiDropdownList::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
    main = c;
    main.set(CPR_X,0);
    main.set(CPR_Y,0);
    main.set(CPR_WIDTH,this->w);
    main.set(CPR_HEIGHT,this->h);
    plyList->configure(config.getUInt16(CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST),this->w,this->h,QColor(main.getColor(CPR_TEXT_COLOR)));
    colorState = ColoUiItem::IS_NORMAL;
}

void ColoUiDropdownList::itemChanged(qint32 currentItem){
    this->signalInfo.data = currentItem;
    this->signalInfo.type = ST_VALUE_CHANGED;
    this->signalSender->sendSignal(signalInfo);
    update();
}

void ColoUiDropdownList::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // Drawing the button of the list
    ColoUiItem item("",NULL);
    if (plyList->selectionEmpty()){
        item.setConfiguration(main);
    }
    else{
        item.setConfiguration(plyList->getCurrentItem());
    }


    item.drawItem(painter,colorState);

    painter->fillPath(plyList->getDropDownIndicatorPath(),QBrush(QColor(config.getColor(CPR_TEXT_COLOR))));

}

void ColoUiDropdownList::mousePressEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    if (!plyList->itemsEmpty() && !plyList->isVisible()){
        this->toggleList(true);
    }
}

void ColoUiDropdownList::hoverEnterEvent(QGraphicsSceneHoverEvent *e){
    Q_UNUSED(e);
    if (colorState == ColoUiItem::IS_NORMAL){
        colorState = ColoUiItem::IS_HOVER;
        update();
    }
}

void ColoUiDropdownList::hoverLeaveEvent(QGraphicsSceneHoverEvent *e){
    Q_UNUSED(e);
    if (plyList->isVisible()){
        if (!plyList->isUnderMouse()){
            this->toggleList(false);
        }
    }
    else{
        colorState = ColoUiItem::IS_NORMAL;
        update();
    }
}

void ColoUiDropdownList::toggleList(bool unfold){
    if (unfold){
        colorState = ColoUiItem::IS_ALTERNATIVE;
    }
    else{
        colorState = ColoUiItem::IS_NORMAL;
    }
    plyList->setVisible(unfold);
    plyList->update();
    this->update();
}


//################################# PLYLIST ###############################################

ColoUiDropdownList::PlyList::PlyList(ColoUiDropdownList *p){

    parent = p;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    this->setAcceptHoverEvents(true);
    currentIndex = -1;
    this->setZValue(10);
    itemToStartDrawing = -1;
}

void ColoUiDropdownList::PlyList::configure(qint32 n, qreal w, qreal h, QColor tColor){
    itemsToShow = n;
    textColor = tColor;

    itemW = w;
    itemH = h;

    directionIndicatorHeight = itemH*0.3;

    qreal arrowH = itemH*0.2;
    qreal arrowW = itemW*0.2;
    qreal xcenter = (itemW - arrowW)/2;
    qreal ycenter = (directionIndicatorHeight - arrowH)/2;
    qreal xleft = itemW/4 - arrowW/2;
    qreal xright = itemW*3/4 - arrowW/2;

    centerArrow = QRectF(xcenter,ycenter,arrowW,arrowH);
    arrowLeft = QRectF(xleft,ycenter,arrowW,arrowH);
    arrowRight = QRectF(xright,ycenter,arrowW,arrowH);

    qreal dim = h*0.3;

    QRectF ddi (w*0.98-dim,h*0.95 - dim,dim,dim);
    dropDownIndicator = drawArrow(ddi,TT_AT_45);

    itemToStartDrawing = -1;

    updateBoundingBox();

}

void ColoUiDropdownList::PlyList::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (items.isEmpty()) return;

    // Drawing the dropdown indicator
    painter->fillPath(dropDownIndicator,QBrush(textColor));

    quint16 y = 0;
    qint32 itemStart;
    if (itemToStartDrawing == -1) itemStart = 0;
    else itemStart = itemToStartDrawing;

    qint32 N = qMin(itemStart+itemsToShow,items.size());

    ColoUiItem::ItemState state;

    for (qint32 i = itemStart; i < N; i++){
        ColoUiConfiguration c = items.at(i);
        c.set(CPR_Y,y);
        ColoUiItem item("",NULL);
        item.setConfiguration(c);
        if (i == hoverItem){
            state = ColoUiItem::IS_ALTERNATIVE;
        }
        else{
            state = ColoUiItem::IS_NORMAL;
        }
        item.drawItem(painter,state);
        y = y + itemH;
    }

    // Drawing the up/down indicator
    QRectF indicator(0,y,itemW,this->directionIndicatorHeight);
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(indicator);

    if (items.size() > itemsToShow){
        if (itemStart > 0){
            if (itemStart >= (items.size() - itemsToShow)){
                QRectF r = centerArrow;
                r.moveTop(r.top() + y);
                painter->fillPath(drawArrow(r,TT_UP),QBrush(QColor(Qt::white)));
            }
            else{
                // Draw both
                QRectF r = arrowLeft;
                r.moveTop(r.top() + y);
                painter->fillPath(drawArrow(r,TT_UP),QBrush(QColor(Qt::white)));
                r = arrowRight;
                r.moveTop(r.top() + y);
                painter->fillPath(drawArrow(r,TT_DOWN),QBrush(QColor(Qt::white)));
            }
        }
        else{
            QRectF r = centerArrow;
            r.moveTop(r.top() + y);
            painter->fillPath(drawArrow(r,TT_DOWN),QBrush(QColor(Qt::white)));
        }
    }
}

void ColoUiDropdownList::PlyList::updateItemToDraw(bool up){
    if (up){
        if (itemToStartDrawing > 0) itemToStartDrawing--;
    }
    else{
        qint32 N = items.size();

        if (itemsToShow >= N){
            itemToStartDrawing = 0;
            return;
        }

        if (itemToStartDrawing < N-itemsToShow){
            itemToStartDrawing++;
        }
    }
    update();
}


ColoUiConfiguration ColoUiDropdownList::PlyList::getItem(qint32 id) const{
    if ((id >=0 ) && (id < items.size())){
        return items.at(id);
    }
    else return ColoUiConfiguration();
}

ColoUiConfiguration ColoUiDropdownList::PlyList::getCurrentItem() const{
    if (currentIndex != -1){
        return items.at(currentIndex);
    }
    else return ColoUiConfiguration();
}

void ColoUiDropdownList::PlyList::removeItem(qint32 index){
    if ((index >= 0) && (index < items.size())){
        currentIndex = -1;
        items.remove(index);
        updateBoundingBox();
    }
}

void ColoUiDropdownList::PlyList::clearItems(){
    currentIndex = -1;
    items.clear();
    updateBoundingBox();
}

void ColoUiDropdownList::PlyList::addItem(ColoUiConfiguration id){
    id.set(CPR_WIDTH,itemW);
    id.set(CPR_HEIGHT,itemH);
    id.set(CPR_X,0);
    items << id;
    updateBoundingBox();
    update();
}

void ColoUiDropdownList::PlyList::addItem(QString text){
    ColoUiConfiguration c;
    if (!items.isEmpty()){
        c = items.last();
    }
    c.set(CPR_WIDTH,itemW);
    c.set(CPR_HEIGHT,itemH);
    c.set(CPR_X,0);
    c.set(CPR_TEXT,text);
    updateBoundingBox();
    update();
}

void ColoUiDropdownList::PlyList::clearSelection(){
    currentIndex = -1;
    updateBoundingBox();
    update();
}

void ColoUiDropdownList::PlyList::updateBoundingBox(){
    qreal N = qMin(items.size(),itemsToShow);
    boundingBox = QRectF(0,0,itemW,(N+1)*itemH + directionIndicatorHeight);
}

void ColoUiDropdownList::PlyList::mousePressEvent(QGraphicsSceneMouseEvent *e){
    // This is for scrolling
    lastY = e->pos().y();
    accDeltaY = 0;
}

void ColoUiDropdownList::PlyList::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
    // Touch/Mouse scrolling the list
    accDeltaY = accDeltaY + lastY - e->pos().y();
    lastY = e->pos().y();
    if (qAbs(accDeltaY) >= (itemH*0.8)){
        updateItemToDraw(accDeltaY < 0);
        accDeltaY = 0;
    }
}

void ColoUiDropdownList::PlyList::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    currentIndex = hoverItem;

    parent->itemChanged(currentIndex);

    // Drawing just the header
    parent->toggleList(false);
}

void ColoUiDropdownList::PlyList::hoverLeaveEvent(QGraphicsSceneHoverEvent *e){
    Q_UNUSED(e);
    parent->toggleList(false);
}


void ColoUiDropdownList::PlyList::hoverMoveEvent(QGraphicsSceneHoverEvent *e){
    quint16 hoverOn = e->pos().y()/itemH;    
    if (itemToStartDrawing == -1){
        hoverItem = hoverOn;
    }
    else{
        hoverItem = itemToStartDrawing + hoverOn;
    }
    update();
}

void ColoUiDropdownList::PlyList::wheelEvent(QGraphicsSceneWheelEvent *e){
    updateItemToDraw(e->delta() > 0);
}


QPainterPath ColoUiDropdownList::PlyList::drawArrow(QRectF r, TriangleType tt){

    QPainterPath path;
    QPointF start;

    switch(tt){
    case TT_DOWN:
        start.setX(r.left() + r.width()/2);
        start.setY(r.bottom());
        path.moveTo(start);
        path.lineTo(r.topLeft());
        path.lineTo(r.topRight());
        path.lineTo(start);
        break;
    case TT_UP:
        start.setX(r.left() + r.width()/2);
        start.setY(r.top());
        path.moveTo(start);
        path.lineTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        path.lineTo(start);
        break;
    case TT_AT_45:
        path.moveTo(r.bottomRight());
        path.lineTo(r.topRight());
        path.lineTo(r.bottomLeft());
        path.lineTo(r.bottomRight());
        break;
    }

    return path;

}
