#include "colouicontainer.h"

ColoUiContainer::ColoUiContainer()
{
    //elementID = name;
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    inputDialog = new ColoUiTextInputDialog(this);

    //connect(signalManager,&ColoUiSignalManager::signalTriggered,this,&ColoUiContainer::on_coloUiSignal);
    connect(&resizeEventTimer,&QTimer::timeout,this,&ColoUiContainer::on_resizeEventDone);


    // Intializing the drawAreaRect
    drawAreaRect = new ColoUiDrawingGrid();

    // Default drawing area is 1000x1000
    setDrawingArea(1000,1000);

    drawDrawAreaRect = false;

    forceNoScrollBars = false;

}

void ColoUiContainer::on_coloUiSignal(){

}


QString ColoUiContainer::createView(QString ID, quint16 x, quint16 y, quint16 w, quint16 h){

    if (views.contains(ID)){
        return ERROR_VIEW_NAME_IN_USE;
    }

    // Checking for collisions
    QRect rect(x,y,w,h);
    QHashIterator<QString,ColoUiView*> i(views);
    while (i.hasNext()){
        if (rect.intersects(i.value()->getViewRect())){
            QString error = ERROR_VIEW_OVERLAPS;
            error = error  + "_" + ID;
            return error;
        }
    }


    // Checking for dimensions
    if ((x > SCREEN_WIDTH) || (x + w > SCREEN_WIDTH)){
        return ERROR_VIEW_NOT_CONTAINED_IN_DRAWING_AREA;
    }
    if ((y > SCREEN_HEIGHT) || (y + h > SCREEN_HEIGHT)){
        return ERROR_VIEW_NOT_CONTAINED_IN_DRAWING_AREA;
    }

    ColoUiView *view = new ColoUiView(ID,x,y,w,h,inputDialog);
    views[ID] = view;

    return "";

}


void ColoUiContainer::setDrawingArea(quint16 width, quint16 height){
    SCREEN_HEIGHT = height;
    SCREEN_WIDTH = width;
    drawAreaRect->refreshGrid();
    resizeSceneRect();
}

void ColoUiContainer::setForceNoScrollBars(bool isTrue){
    forceNoScrollBars = isTrue;
    resizeSceneRect();
}

void ColoUiContainer::setDrawDrawAreaRect(bool enable){
    drawDrawAreaRect = enable;
    if (drawDrawAreaRect){
        this->scene()->addItem(drawAreaRect);
        drawAreaRect->setZValue(-1);
        drawAreaRect->setScale(currentScale);
    }
    else{
        this->scene()->removeItem(drawAreaRect);
    }
}



void ColoUiContainer::resizeEvent(QResizeEvent *e){    
    if (resizeEventTimer.isActive()){
        resizeEventTimer.stop();
    }
    resizeEventTimer.start(RESIZE_EVENT_TIMER_MS);
    QGraphicsView::resizeEvent(e);
}


void ColoUiContainer::on_resizeEventDone(){
    resizeEventTimer.stop();
    resizeSceneRect();
}

void ColoUiContainer::showEvent(QShowEvent *e){
    resizeSceneRect();
    QGraphicsView::showEvent(e);
}

void ColoUiContainer::resizeSceneRect(){
    qreal w = this->width();
    qreal h = this->height();

    if (!forceNoScrollBars){
        this->scene()->setSceneRect(0,0,w,h);
        this->fitInView(0,0,w,h,Qt::KeepAspectRatio);

        // Selecting the appropiate scale
        currentScale = h/SCREEN_HEIGHT;
        if (SCREEN_WIDTH*currentScale > w){
            currentScale = w/SCREEN_WIDTH;
        }
    }
    else{
        currentScale = qMax(h/SCREEN_HEIGHT,w/SCREEN_WIDTH);
        this->scene()->setSceneRect(0,0,SCREEN_WIDTH*currentScale,SCREEN_HEIGHT*currentScale);
    }

    drawAreaRect->setScale(currentScale);

    // Scaling all items in list
    QList<QGraphicsItem*> list = this->scene()->items();
    for (qint32 i = 0; i < list.size(); i++){
        list.at(i)->setScale(currentScale);
    }
}
