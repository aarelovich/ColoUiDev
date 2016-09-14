#include "colouicontainer.h"

ColoUiContainer::ColoUiContainer()
{
    //elementID = name;
    signalManager = new ColoUiSignalManager();
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    inputDialog = new ColoUiTextInputDialog(this);

    connect(signalManager,&ColoUiSignalManager::signalTriggered,this,&ColoUiContainer::on_coloUiSignal);
    connect(&resizeEventTimer,&QTimer::timeout,this,&ColoUiContainer::on_resizeEventDone);


    // Intializing the drawAreaRect
    drawAreaRect = new QGraphicsRectItem();
    QPen pen;
    pen.setWidth(0);
    drawAreaRect->setPen(pen);
    drawAreaRect->setBrush(QBrush(Qt::gray));

    // Default drawing area is 1000x1000
    setDrawingArea(1000,1000);

    drawDrawAreaRect = false;

}

void ColoUiContainer::setDrawingArea(qreal width, qreal height){
    SCREEN_HEIGHT = height;
    SCREEN_WIDTH = width;
    drawAreaRect->setRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
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

void ColoUiContainer::on_coloUiSignal(){

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
    this->scene()->setSceneRect(0,0,w,h);
    this->fitInView(0,0,w,h,Qt::KeepAspectRatio);

    // Selecting the appropiate scale
    currentScale = h/SCREEN_HEIGHT;
    if (SCREEN_WIDTH*currentScale > w){
        currentScale = w/SCREEN_WIDTH;
    }

    drawAreaRect->setScale(currentScale);

    // Scaling all items in list
    QList<QGraphicsItem*> list = this->scene()->items();
    for (qint32 i = 0; i < list.size(); i++){
        list.at(i)->setScale(currentScale);
    }
}
