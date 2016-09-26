#include "colouicontainer.h"

ColoUiContainer::ColoUiContainer()
{
    //elementID = name;
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    inputDialog = new ColoUiTextInputDialog(this);
    uiHasBeenDrawn = false;

    // The resize event timer
    connect(&resizeEventTimer,&QTimer::timeout,this,&ColoUiContainer::on_resizeEventDone);

    // The animation timer
    connect(&transitionTimer,&QTimer::timeout,this,&ColoUiContainer::on_transitionTimerTimeout);

    // Creating the signal manager
    signalManager = new ColoUiSignalManager(this);

    // Default drawing area is 1000x1000
    setDrawingArea(1000,1000);

    // The transition screen
    transitionScreen = new ColoUiTransitionScreen();

    forceNoScrollBars = false;

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

}


QString ColoUiContainer::createView(QString ID, quint16 x, quint16 y, quint16 w, quint16 h, bool dimensionsAreRelative){

    if (dimensionsAreRelative){
        x = qMin(100.0,qreal(x))*(qreal)SCREEN_WIDTH/100.0;
        w = qMin(100.0,qreal(w))*(qreal)SCREEN_WIDTH/100.0;
        y = qMin(100.0,qreal(y))*(qreal)SCREEN_HEIGHT/100.0;
        h = qMin(100.0,qreal(h))*(qreal)SCREEN_HEIGHT/100.0;
    }

    if (views.contains(ID)){
        return ERROR_VIEW_NAME_IN_USE;
    }

    if (ID.contains(".")){
        return ERROR_NAMES_CANNOT_CONTAIN_DOT;
    }

    // Checking for collisions
    QRect rect(x,y,w,h);
    QList<QString> keys = views.keys();

    bool drawnView = true;

    for (qint32 i = 0; i < keys.size(); i++){
        if (rect.intersects(views.value(keys.at(i))->getViewRect())){

            // Need to check that its data is not EXACTLY the same
            if (rect != views.value(keys.at(i))->getViewRect()){
                QString error = ERROR_VIEW_OVERLAPS;
                error = error  + "_" + ID;
                return error;
            }
            else{
                // If the views match exactly, then this is a second view for this position
                drawnView = false;
            }

        }
    }

    // Checking for dimensions
    if (x + w > SCREEN_WIDTH){
        return ERROR_VIEW_NOT_CONTAINED_IN_DRAWING_AREA;
    }
    if (y + h > SCREEN_HEIGHT){
        return ERROR_VIEW_NOT_CONTAINED_IN_DRAWING_AREA;
    }

    ColoUiView *view = new ColoUiView(ID,x,y,w,h,inputDialog);
    views[ID] = view;
    drawnViews[ID] = drawnView;

    return "";

}

QString ColoUiContainer::addTransition(ColoUiConfiguration t){

    // Making sure the transition is between two valid views.
    QString viewA = t.getString(CPR_TRANSITION_VIEW_A);
    QString viewB = t.getString(CPR_TRANSITION_VIEW_B);

    if (!views.contains(viewA)) return ERROR_VIEWA_NOT_FOUND;
    if (!views.contains(viewB)) return ERROR_VIEWB_NOT_FOUND;

    if (views.value(viewA)->getViewRect() != views.value(viewB)->getViewRect())
        return ERROR_VIEW_SIZES_ARE_DIFFERENT;

    // Checking if transition doesn't exist

    qint32 tid = -1;

    for (qint32 i = 0; i < transitions.size(); i++){

        QString tviewA = transitions.at(i).getString(CPR_TRANSITION_VIEW_A);
        QString tviewB = transitions.at(i).getString(CPR_TRANSITION_VIEW_B);

        if ((tviewA == viewA && tviewB == viewB) ||
            (tviewA == viewB && tviewB == viewA) ){
            tid = i;
            break;
        }
    }

    if (tid == -1){
        // Adding the transtion
        transitions << t;
        return "";
    }
    else{
        return ERROR_TRANSITION_EXISTS;
    }

}

void ColoUiContainer::startTranstion(QString viewA, QString viewB){

    // Finding the transtion
    qint32 tid = -1;

    for (qint32 i = 0; i < transitions.size(); i++){

        QString tviewA = transitions.at(i).getString(CPR_TRANSITION_VIEW_A);
        QString tviewB = transitions.at(i).getString(CPR_TRANSITION_VIEW_B);

        if ((tviewA == viewA && tviewB == viewB) ||
            (tviewA == viewB && tviewB == viewA) ){
            tid = i;
            break;
        }
    }

    qDebug() << "Tid" << tid;

    if (tid == -1) return; // No transition for these two views

    // Only one fo the views should be drawn
    ColoUiView *toInsert;
    if (drawnViews.value(viewA)){
        // Transition from A to B
        drawnViews[viewA] = false;
        drawnViews[viewB] = true;
        toInsert = views.value(viewB);
        viewToInsert = viewB;
        viewToRemove = viewA;
    }
    else{
        // Transition from B to A
        drawnViews[viewA] = true;
        drawnViews[viewB] = false;
        toInsert = views.value(viewA);
        viewToInsert = viewA;
        viewToRemove = viewB;
    }

    toInsert->drawView(this->scene());

    // Depending on where it transitons from, the viewToInsert is drawn.
    qreal dimension = 0;

    QRect r = toInsert->getViewRect();

    switch (transitions.at(tid).getUInt16(CPR_TRANSITION_TYPE)){
    case CPA_DOWN:
        dimension = -r.height();
        toInsert->translateView(dimension,false);
        activeTransitionInX = false;
        break;
    case CPA_LEFT:
        dimension = r.width();
        toInsert->translateView(dimension,true);
        activeTransitionInX = true;
        break;
    case CPA_RIGHT:
        dimension = r.height();
        toInsert->translateView(dimension,false);
        activeTransitionInX = false;
        break;
    default:
        dimension = -r.width();
        toInsert->translateView(dimension,true);
        activeTransitionInX = true;
        break;
    }

    transitionScreen->setViewWindow(r.x(),r.y(),r.width(),r.height());

    activeTransitionCounter = transitions.at(tid).getUInt16(CPR_TRANSITION_STEPS);
    activeTransitionDelta = -dimension/(qreal)activeTransitionCounter;
    qint32 ms = transitions.at(tid).getUInt16(CPR_TRANSITION_TIME)/activeTransitionCounter;
    transitionTimer.setInterval(ms);    
    transitionTimer.start();

}


void ColoUiContainer::setDrawingArea(quint16 width, quint16 height){
    SCREEN_HEIGHT = height;
    SCREEN_WIDTH = width;
    resizeSceneRect();
}

void ColoUiContainer::setForceNoScrollBars(bool isTrue){
    forceNoScrollBars = isTrue;
    resizeSceneRect();
}


ColoUiView* ColoUiContainer::getViewByID(QString id) const{
    if (views.contains(id)){
        return views.value(id);
    }
    else return NULL;
}

ColoUiElement* ColoUiContainer::element(QString id) const{

    // The element name should be its compound name otherwise it can't be found.
    QStringList parts = id.split(".");
    QString viewID = parts.first();
    if (views.contains(viewID)){
        return views.value(viewID)->element(id);
    }

    return NULL;

}

void ColoUiContainer::on_transitionTimerTimeout(){
    views.value(viewToInsert)->translateView(activeTransitionDelta,activeTransitionInX);
    views.value(viewToRemove)->translateView(activeTransitionDelta,activeTransitionInX);
    activeTransitionCounter--;
    if (activeTransitionCounter == 0){
        transitionTimer.stop();
        views.value(viewToRemove)->removeView(this->scene());
        views.value(viewToRemove)->resetDeltasAndZValue();
        views.value(viewToInsert)->resetDeltasAndZValue();
    }
    this->scene()->update();
}


void ColoUiContainer::drawUi(){
    if (uiHasBeenDrawn) return; //UI needs to be drawn only once
    QList<QString> keys = views.keys();
    for (int i= 0; i < keys.size(); i++){
        ColoUiView *view = views.value(keys.at(i));
        if (drawnViews.value(keys.at(i))){ // Only draw the front views. Transitions will draw the rest.
            view->drawView(this->scene());
        }
    }
    scene()->addItem(transitionScreen);
    uiHasBeenDrawn = true;
    this->scene()->update();
}

void ColoUiContainer::deleteUi(){
    this->scene()->clear();
    views.clear();
    drawnViews.clear();
    transitions.clear();
    uiHasBeenDrawn = false;
    // Recreating the transition screen
    transitionScreen = new ColoUiTransitionScreen();

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
    this->scene()->setSceneRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    if (!forceNoScrollBars){
        this->fitInView(this->scene()->sceneRect(),Qt::KeepAspectRatioByExpanding);
    }
    else{
        this->fitInView(this->scene()->sceneRect(),Qt::IgnoreAspectRatio);
    }

}
