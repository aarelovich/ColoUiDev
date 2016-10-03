#include "colouilist.h"

ColoUiList::ColoUiList(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    this->type = CUI_LIST;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    yStartPoint = 0;
    showHeaders = true;
    this->setAcceptHoverEvents(true);
    justSentDClick = 0;
}

void ColoUiList::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
    itemH = this->h/config.getUInt16(CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST);
    showHeaders = c.getBool(CPR_LIST_HEADER_VISIBLE);
    //qDebug() << "Show headers is" << showHeaders;
    update();
}

qint32 ColoUiList::getColCount() const{
    return headers.size();
}

qint32 ColoUiList::getRowCount() const{
    return items.size();
}

ColoUiConfiguration ColoUiList::getHeaderConfig(quint32 col){
    if (col < (quint32)headers.size()){
        return headers.at(col);
    }
    return ColoUiConfiguration();
}

bool ColoUiList::setHeaderConfig(ColoUiConfiguration c, qint32 col){
    if ((col < headers.size()) && (col >= 0)){
        headers[col] =  c;
        update();
        return true;
    }
    else if (col == -1){

        headers << c;

        // Resetting the columns width.
        quint16 tempWidth = this->w/headers.size();
        for (qint32 i = 0; i < headers.size(); i++){
            headers[i].set(CPR_WIDTH,tempWidth);
        }

        // Adding an empty configuration to existing rows in the last column
        for (qint32 i = 0; i < items.size(); i++){
            items[i] << ColoUiConfiguration();
        }

        return true;
    }
    else return false;
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

bool ColoUiList::setItemConfiguration(quint32 row, quint32 col, ColoUiConfiguration c){
    if (row < (quint32)items.size()){
        if (col < (quint32)items.at(row).size()){
            items[row][col] = c;
            update();
            return true;
        }
    }
    return false;
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

    QVector<ColoUiConfiguration> row;
    for (qint32 i = 0; i < headers.size(); i++){
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

    qint32 xvalue = 0;

    //qDebug() << "SHOW HEADERS";
    if (showHeaders){
        xvalue = 0;
        for (qint32 j = 0; j < headers.size(); j++){

            ColoUiItem item("",NULL);
            ColoUiConfiguration c = headers.at(j);

            // Configuring the item geometr.
            c.set(CPR_Y,0);
            c.set(CPR_X,xvalue);
            c.set(CPR_HEIGHT,itemH);

            //qDebug() << "Header x" << xvalue << "Item height" << itemH <<  "Width" << c.getUInt16(CPR_WIDTH) << " Text " << c.getString(CPR_TEXT);

            // Drawing the items.
            item.setConfiguration(c);
            item.drawItem(painter);

            xvalue = xvalue + c.getUInt16(CPR_WIDTH);
        }
    }

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
    xvalue = 0;

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
    xLastColoWidthPoint = now.x();
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
    if (!resizeColumns.isEmpty()){
        qreal dx = xLastColoWidthPoint - now.x();
        xLastColoWidthPoint = now.x();
        //qDebug() << "DX is" << dx;

        quint16 col1w = headers.at(resizeColumns.first()).getUInt16(CPR_WIDTH);
        quint16 col2w = headers.at(resizeColumns.last()).getUInt16(CPR_WIDTH);
        quint16 dix;

        //qDebug() << "Col 1 and 2 and sum" << col1w << col2w << col1w + col2w;

        if (dx > 0){
            if ((col1w - dx) <= MIN_COL_WIDTH){
                dx = col1w - MIN_COL_WIDTH;
            }
            dix = dx; // Making it whole so the sum of col1w and col2w does not change
            col1w = col1w - dix;
            col2w = col2w + dix;
        }
        else{
            dx = -dx;
            if ((col2w - dx) <= MIN_COL_WIDTH){
                dx = col2w - MIN_COL_WIDTH;
            }
            dix = dx;  // Making it whole so the sum of col1w and col2w does not change
            col1w = col1w + dix;
            col2w = col2w - dix;
        }

        headers[resizeColumns.first()].set(CPR_WIDTH,col1w);
        headers[resizeColumns.last()].set(CPR_WIDTH,col2w);
        update();
    }

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

    resizeColumns.clear();

    if (!config.getBool(CPR_ALTERNATIVE_BACKGROUND_ON_HOVER) && !showHeaders) return;

    qreal diffx = 50; // This value will not cuse a change in icon.
    QPoint p = getRowAndColForClick(e->pos(),&diffx);

    qint32 row = ((e->pos().y()+yStartPoint)/itemH);
    qint32 col = p.y();
    if (showHeaders){
        if (row == 0){
            //qDebug() << "On header with diff" << diffx;
            if ((diffx < 2) && (col < headers.size()-1)){
                resizeColumns << col << col+1;

            }
            else if ((diffx > 98) && (col > 0)){
                resizeColumns << col-1 << col;
            }
        }
        row = row - 1;
    }
    hoverRow = p.x();
    if (!resizeColumns.isEmpty()){
        this->setCursor(QCursor(Qt::SplitHCursor));
        //qDebug() << "Should resize columns" << resizeColumns;
    }
    else{
        this->setCursor(QCursor(Qt::ArrowCursor));

    }
    update();
    QGraphicsItem::hoverMoveEvent(e);
}

void ColoUiList::hoverLeaveEvent(QGraphicsSceneHoverEvent *e){
    Q_UNUSED(e);
    hoverRow = -2;
    update();
}

QPoint ColoUiList::getRowAndColForClick(QPointF mouse, qreal *diffx){

    qint32 row = ((mouse.y()+yStartPoint)/itemH);
    if (showHeaders){
        if (row == 0){
            qreal acc_w = 0;
            qint32 col = 0;
            for (qint32 i = 0; i < headers.size(); i++){
                acc_w = acc_w + headers.at(i).getUInt16(CPR_WIDTH);
                if (mouse.x() < acc_w){
                    if (diffx != nullptr){
                        *diffx = (acc_w - mouse.x())*100.0/(qreal)headers.at(i).getUInt16(CPR_WIDTH);
                        //qDebug() << "Diff X" << *diffx << acc_w << mouse.x() << headers.at(i).getUInt16(CPR_WIDTH);
                    }
                    col = i;
                    break;
                }
            }
            return QPoint(-1,col);
        }
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

