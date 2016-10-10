#include "colouidropdownlist.h"

ColoUiDropdownList::ColoUiDropdownList(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    this->type = CUI_DROPDOWN;
    dropped = false;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    this->setAcceptHoverEvents(true);
    currentIndex = -1;
}


void ColoUiDropdownList::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
    main = c;
    main.set(CPR_X,0);
    main.set(CPR_Y,0);
    main.set(CPR_WIDTH,this->w);
    main.set(CPR_HEIGHT,this->h);
    directionIndicatorHeight = this->h*0.3;

    qreal arrowH = this->h*0.2;
    qreal arrowW = this->w*0.2;
    qreal xcenter = (w - arrowW)/2;
    qreal ycenter = (directionIndicatorHeight - arrowH)/2;
    qreal xleft = w/4 - arrowW/2;
    qreal xright = w*3/4 - arrowW/2;

    centerArrow = QRectF(xcenter,ycenter,arrowW,arrowH);
    arrowLeft = QRectF(xleft,ycenter,arrowW,arrowH);
    arrowRight = QRectF(xright,ycenter,arrowW,arrowH);

    qreal dim = h*0.3;

    QRectF ddi (w*0.98-dim,h*0.95 - dim,dim,dim);
    dropDownIndicator = drawArrow(ddi,TT_AT_45);

}

void ColoUiDropdownList::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // Drawing the button of the list
    ColoUiItem item("",NULL);

    if (!items.isEmpty()){
        if (currentIndex > -1){
            item.setConfiguration(items.at(currentIndex));
        }
        else{
            item.setConfiguration(main);
        }
    }

    item.drawItem(painter,dropped);

    // Drawing the dropdown indicator
    painter->fillPath(dropDownIndicator,QBrush(QColor(main.getColor(CPR_TEXT_COLOR))));

    if (!dropped){
        //this->setZValue(this->zValue()+1);
        boundingBox = QRectF(0,0,this->w,this->h);
        return;
    }

    quint16 y = this->h;
    qint32 itemStart;
    if (itemToStartDrawing == -1) itemStart = 0;
    else itemStart = itemToStartDrawing;

    qint32 nitems = config.getUInt16(CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST);
    qint32 N = qMin(itemStart+nitems,items.size());

    for (qint32 i = itemStart; i < N; i++){
        ColoUiConfiguration c = items.at(i);
        c.set(CPR_Y,y);
        ColoUiItem item("",NULL);
        item.setConfiguration(c);
        item.drawItem(painter,i == hoverItem);
        y = y + this->h;
    }

    // Drawing the up/down indicator
    QRectF indicator(0,y,this->w,this->directionIndicatorHeight);
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(indicator);

    if (items.size() > nitems){
        if (itemStart > 0){
            if (itemStart >= (items.size() - nitems)){
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

void ColoUiDropdownList::mousePressEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    if (!dropped){

        if (items.isEmpty()) return; // Nothign to do

        dropped = true;

        // Calculating the new bounding box
        if (items.size() > 0){
            qreal N = qMin(items.size(),config.getInt32(CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST));            
            boundingBox = QRectF(0,0,this->w,(N+1)*this->h + directionIndicatorHeight);
            itemToStartDrawing = currentIndex;
            hoverItem = -1;
            this->setZValue(this->zValue()+1);
        }

        update();
    }
    else{
        // This is for scrolling
        lastY = e->pos().y();
        accDeltaY = 0;
    }
}

void ColoUiDropdownList::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
    if (dropped){
        accDeltaY = accDeltaY + lastY - e->pos().y();
        lastY = e->pos().y();
        if (qAbs(accDeltaY) >= (this->h*0.8)){
            updateItemToDraw(accDeltaY < 0);
            accDeltaY = 0;
        }
    }
}

void ColoUiDropdownList::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
}

void ColoUiDropdownList::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    currentIndex = hoverItem;

    // Signal that selected item was changed.
    this->signalInfo.type = ST_MOUSE_DOUBLE_CLICK;
    this->signalInfo.data = currentIndex;
    this->signalSender->sendSignal(this->signalInfo);

    // Drawing just the header
    ply();
}

void ColoUiDropdownList::hoverLeaveEvent(QGraphicsSceneHoverEvent *e){
    Q_UNUSED(e)    
    ply();
}

void ColoUiDropdownList::ply(){
    if (dropped)
        this->setZValue(this->zValue()-1);
    dropped = false;
    update();
}

void ColoUiDropdownList::hoverMoveEvent(QGraphicsSceneHoverEvent *e){
    if (dropped){
        quint16 hoverOn = e->pos().y()/this->h;
        if (hoverOn > 0){
            if (itemToStartDrawing == -1){
                hoverItem = hoverOn - 1;
            }
            else{
                hoverItem = itemToStartDrawing + hoverOn - 1;
            }
            update();
        }
    }
}

void ColoUiDropdownList::wheelEvent(QGraphicsSceneWheelEvent *e){
    if (dropped){
        updateItemToDraw(e->delta() > 0);
    }
}

ColoUiConfiguration ColoUiDropdownList::getItem(qint32 id) const{
    if ((id >=0 ) && (id < items.size())){
        return items.at(id);
    }
    else return ColoUiConfiguration();
}

ColoUiConfiguration ColoUiDropdownList::getCurrentItem() const{
    if (currentIndex == -1){
        return items.at(currentIndex);
    }
    else return ColoUiConfiguration();
}

void ColoUiDropdownList::removeItem(qint32 index){
    if ((index >= 0) && (index < items.size())){
        currentIndex = -1;
        items.remove(index);
        ply();
    }
}

void ColoUiDropdownList::clearItems(){
    currentIndex = -1;
    items.clear();
    ply();
}

void ColoUiDropdownList::addItem(ColoUiConfiguration id){
    id.set(CPR_WIDTH,this->w);
    id.set(CPR_HEIGHT,this->h);
    id.set(CPR_X,0);
    items << id;
    update();
}

void ColoUiDropdownList::addItem(QString text){
    ColoUiConfiguration c;
    if (!items.isEmpty()){
        c = items.last();
    }
    c.set(CPR_WIDTH,this->w);
    c.set(CPR_HEIGHT,this->h);
    c.set(CPR_X,0);
    c.set(CPR_TEXT,text);
    update();
}

void ColoUiDropdownList::clearSelection(){
    currentIndex = -1;
    update();
}

void ColoUiDropdownList::updateItemToDraw(bool up){
    if (up){
        if (itemToStartDrawing > 0) itemToStartDrawing--;
    }
    else{
        qint32 M = config.getInt32(CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST);
        qint32 N = items.size();

        if (M >= N){
            itemToStartDrawing = 0;
            return;
        }

        if (itemToStartDrawing < N-M){
            itemToStartDrawing++;
        }
    }
    update();
}

QPainterPath ColoUiDropdownList::drawArrow(QRectF r, TriangleType tt){

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
