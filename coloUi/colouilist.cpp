#include "colouilist.h"

ColoUiList::ColoUiList(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    this->type = CUI_LIST;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    yStartPoint = 0;
    showHeaders = false;
    this->setAcceptHoverEvents(true);
    justSentDClick = 0;
}

void ColoUiList::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);

    itemH = this->h/config.getUInt16(CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST);
    QStringList headerList = config.getStringList(CPR_LIST_HEADERS);

    // If the number of columns has changed data is cleared.
    if ((headerList.size() != items.size()) && (!headerList.isEmpty())){
        items.clear();
    }

    // Checking if the headers should be drawn.
    showHeaders = false;
    for (qint32 i = 0; i < headerList.size(); i++){
        if (!headerList.at(i).isEmpty()){
            showHeaders = true;
            break;
        }
    }

    // Setting the width to be equal for all items and their text
    for (qint32 i = 0; i < headerList.size(); i++){
        ColoUiConfiguration c;
        c.set(CPR_TEXT,headerList.at(i));
        c.set(CPR_WIDTH,this->w/headerList.size());
        headers << c;
    }


    update();

}

qint32 ColoUiList::getColCount() const{
    return headers.size();
}

qint32 ColoUiList::getRowCount() const{
    return items.size();
}

ColoUiConfiguration ColoUiList::getHeaderConfig(quint32 col){
    if (col < (quint32)config.getStringList(CPR_LIST_HEADERS).size()){
        return headers.at(col);
    }
    return ColoUiConfiguration();
}

void ColoUiList::setHeaderConfig(quint32 col, ColoUiConfiguration c, bool colWidthIsAbsolute){
    if (col < (quint32)config.getStringList(CPR_LIST_HEADERS).size()){
        if (!colWidthIsAbsolute){
            c.set(CPR_WIDTH,this->w*c.getUInt16(CPR_WIDTH)/100);
        }
        headers[col] =  c;
        update();
    }
}

ColoUiConfiguration ColoUiList::getItemConfiguration(quint32 row, quint32 col){
    if (row < (quint32)items.size()){
        if (col < (quint32)items.at(row).size()){
            return items.at(row).at(col);
        }
        else{
            return ColoUiConfiguration();
        }
    }
    else{
        return ColoUiConfiguration();
    }
}

void ColoUiList::setItemConfiguration(quint32 row, quint32 col, ColoUiConfiguration c){
    if (row < (quint32)items.size()){
        if (col < (quint32)items.at(row).size()){
            items[row][col] = c;
            update();
        }
    }
}

void ColoUiList::insertRow(qint32 where){

    qint32 pos;
    if (where < 0){
        pos = items.size();
    }
    else{
        if (where <= items.size()){
            pos = where;
        }
        else return;
    }

    qint32 N = 1;
    if (!config.getStringList(CPR_LIST_HEADERS).empty()){
        N = config.getStringList(CPR_LIST_HEADERS).size();
    }
    QVector<ColoUiConfiguration> row;
    for (qint32 i = 0; i < N; i++){
        row << ColoUiConfiguration();
    }

    items.insert(pos,row);

    maxYDisplacement = items.size()*itemH - this->h;
    if (maxYDisplacement < 0){
        maxYDisplacement = 0;
    }
    else{
        if (showHeaders){
            // Needs an extra space due to the headers.
            maxYDisplacement = maxYDisplacement + itemH;
        }
    }

}


void ColoUiList::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget)
    Q_UNUSED(option)


    if (items.empty()) return;

    qint32 itemIndex = qCeil(yStartPoint/itemH);
    qint32 itemStart;
    qint32 itemEnd;
    qint32 N;
    if (showHeaders){
        N = config.getUInt16(CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST)-1;
    }
    else{
        N = config.getUInt16(CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST);
    }

    if (itemIndex == 0){
        itemStart = itemIndex;
        itemEnd = N+1;
    }
    else{
        itemStart = itemIndex -1;
        itemEnd = itemIndex + N;
    }

    qint32 yvalue = -yStartPoint + (qMax(itemIndex-1,0)*itemH);
    if (showHeaders){
        if (itemIndex <= (items.size() - N) || N > items.size()){
            yvalue = yvalue + itemH;
        }
    }
    qint32 xvalue = 0;

    if (itemEnd >= items.size()){
        itemEnd = items.size();
    }

    for (int i = itemStart; i < itemEnd; i++){
        xvalue = 0;
        for (qint32 j = 0;  j < items.at(i).size(); j++){
            ColoUiItem item("",NULL);
            ColoUiConfiguration c = items.at(i).at(j);

            // Configuring the item geometry.
            c.set(CPR_Y,yvalue);
            c.set(CPR_X,xvalue);
            c.set(CPR_WIDTH,headers.at(j).getUInt16(CPR_WIDTH));
            c.set(CPR_HEIGHT,itemH);

            // Drawing the items.
            item.setConfiguration(c);
            if (config.getBool(CPR_ALTERNATIVE_BACKGROUND_ON_HOVER) && hoverRow == i){
                item.drawItem(painter,true);
            }
            else item.drawItem(painter);

            xvalue = xvalue + c.getUInt16(CPR_WIDTH);
        }
        yvalue = yvalue + itemH;
    }

    if (showHeaders){
        xvalue = 0;
        for (qint32 j = 0; j < headers.size(); j++){

            ColoUiItem item("",NULL);
            ColoUiConfiguration c = headers.at(j);

            // Configuring the item geometr.
            c.set(CPR_Y,0);
            c.set(CPR_X,xvalue);
            c.set(CPR_HEIGHT,itemH);

            // Drawing the items.
            item.setConfiguration(c);
            item.drawItem(painter);

            xvalue = xvalue + c.getUInt16(CPR_WIDTH);
        }
    }

}

void ColoUiList::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){

    QPoint p = getRowAndColForClick(e->pos());
    if (p.x() < 0) return;

    justSentDClick = 2;
    signalInfo.data = p;
    signalInfo.type = ST_MOUSE_DOUBLE_CLICK;
    signalSender->sendSignal(signalInfo);

}

void ColoUiList::mousePressEvent(QGraphicsSceneMouseEvent *e){
    QPointF now = mapToScene(e->pos());
    yLastScrollPoint = now.y();
    //qDebug() << "Mouse Pressed";
}

void ColoUiList::wheelEvent(QGraphicsSceneWheelEvent *e){
    // A single mouwheel incremente should be about 20% of the height
    qreal dy = this->h*0.2;
    if (e->delta() > 0){
        dy = -dy;
    }
    updateYStartPoint(dy);
}

void ColoUiList::mouseMoveEvent(QGraphicsSceneMouseEvent *e){

    QPointF now = mapToScene(e->pos());
    qreal dy = yLastScrollPoint - now.y();
    yLastScrollPoint = now.y();
    updateYStartPoint(dy);

}

void ColoUiList::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){

    QPoint p = getRowAndColForClick(e->pos());
    if (p.x() < 0) return;
    if (e->button() == Qt::LeftButton){
        signalInfo.data = p;
        signalInfo.type = ST_MOUSE_CLICK;
        waitForAnotherClick();;
        if (justSentDClick == 0){
            signalSender->sendSignal(signalInfo);
        }
        else{
            justSentDClick--;
        }
    }
    else if (e->button() == Qt::RightButton){
        signalInfo.data = p;
        signalInfo.type = ST_MOUSE_RIGHT_CLICK;
        signalSender->sendSignal(signalInfo);
    }

}

void ColoUiList::hoverMoveEvent(QGraphicsSceneHoverEvent *e){
    if (!config.getBool(CPR_ALTERNATIVE_BACKGROUND_ON_HOVER)) return;

    QPoint p = getRowAndColForClick(e->pos());

    qint32 row = ((e->pos().y()+yStartPoint)/itemH);
    if (showHeaders){
        if (row == 0) return;
        row = row - 1;
    }
    hoverRow = p.x();
    update();
    QGraphicsItem::hoverMoveEvent(e);
}

void ColoUiList::hoverLeaveEvent(QGraphicsSceneHoverEvent *e){
    Q_UNUSED(e);
    hoverRow = -2;
    update();
}

QPoint ColoUiList::getRowAndColForClick(QPointF mouse){

    qint32 row = ((mouse.y()+yStartPoint)/itemH);
    if (showHeaders){
        if (row == 0) return QPoint(-1,-1);
        row = row - 1;
    }

    if (row >= items.size()){
        row = -1;
    }

    qint32 col = 0;
    if (headers.size() > 1){
        qreal acc_w = 0;
        for (qint32 i = 0; i < headers.size(); i++){
            acc_w = acc_w + headers.at(i).getUInt16(CPR_WIDTH);
            if (mouse.x() < acc_w){
                col = i;
                break;
            }
        }
    }

    return QPoint(row,col);

}

void ColoUiList::waitForAnotherClick(){
    QTime dieTime= QTime::currentTime().addMSecs(300);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void ColoUiList::updateYStartPoint(qreal dy){
    yStartPoint = yStartPoint + dy;
    if (yStartPoint > maxYDisplacement){
        yStartPoint = maxYDisplacement;
    }
    else if (yStartPoint < 0){
        yStartPoint = 0;
    }

    update();
}

