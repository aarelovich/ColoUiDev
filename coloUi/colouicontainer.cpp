#include "colouicontainer.h"

ColoUiContainer::ColoUiContainer(QWidget *parent):QGraphicsView(parent)
{
    //elementID = name;
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    uiHasBeenDrawn = false;

    // The resize event timer
    connect(&resizeEventTimer,&QTimer::timeout,this,&ColoUiContainer::on_resizeEventDone);

    // The animation timer
    connect(&transitionTimer,&QTimer::timeout,this,&ColoUiContainer::on_transitionTimerTimeout);

    // The keyboard transtion timer
    connect(&keyboardTransitionTimer,&QTimer::timeout,this,&ColoUiContainer::on_keyboardTranstionTimerTimeout);

    // Creating the signal manager
    signalManager = new ColoUiSignalManager(this);
    connect(signalManager,&ColoUiSignalManager::signalTriggered,this,&ColoUiContainer::on_coloUiSignal);

    // Default drawing area is 1000x1000
    setDrawingArea(1000,1000);

    // The transition screen
    transitionScreen = new ColoUiTransitionScreen();

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    QRect rec = QApplication::desktop()->geometry();
    screenResolution = QSizeF(rec.width(),rec.height());

    keyboardInTransition = false;

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
        if (rect == views.value(keys.at(i))->getViewRect()){
            // If the views match exactly, then this is a second view for this position
            drawnView = false;
        }
    }

    ColoUiView *view = new ColoUiView(ID,x,y,w,h,signalManager);
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

    StringPair p;
    p.first = viewA;
    p.second = viewB;
    awaitingTransitions << p;

    // Transition is called here only if this is the first one.
    if ((awaitingTransitions.size() == 1) && (!keyboardInTransition)){
        doNextTransition();
    }

}

void ColoUiContainer::doNextTransition(){

    if (awaitingTransitions.isEmpty()) return;

    StringPair p = awaitingTransitions.first();
    QString viewA = p.first;
    QString viewB = p.second;

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
    case CPA_UP:
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

    // Creating the keyboard
    softKeyboard = new ColoUiKeyboard(width,height);
    SOFTKEYBOARD_HEIGHT = softKeyboard->boundingRect().height();

    resizeSceneRect();
}

void ColoUiContainer::setTransitionScreenColor(QColor c){
    transitionScreen->setShowColor(c);
    this->scene()->setBackgroundBrush(QBrush(c));
}

ColoUiView* ColoUiContainer::getViewByID(QString id) const{
    if (views.contains(id)){
        return views.value(id);
    }
    else return NULL;
}

bool ColoUiContainer::isViewDrawn(QString viewID) const{
    if (drawnViews.contains(viewID)){
        return drawnViews.value(viewID);
    }
    else return false;
}

ColoUiConfiguration ColoUiContainer::getConfiguration(QString config) const{
    if (parsedConfigs.contains(config)){
        return parsedConfigs.value(config);
    }
    else{
        return ColoUiConfiguration();
    }
}

ColoUiElement* ColoUiContainer::getElement(QString id) const{

    // The element name should be its compound name otherwise it can't be found.
    QStringList parts = id.split(".");
    QString viewID = parts.first();
    if (views.contains(viewID)){
        return views.value(viewID)->getElement(id);
    }

    return NULL;

}

bool ColoUiContainer::replacePlaceHolder(QString placeHolderID, ColoUiElement* customElement){

    QStringList parts = placeHolderID.split(".");
    QString viewID = parts.first();
    if (views.contains(viewID)){
        return views.value(viewID)->replacePlaceHolder(placeHolderID,customElement);
    }
    else return false;

}

QStringList ColoUiContainer::elementList() const {

    QStringList elements;

    QStringList viewList = views.keys();
    for (qint32 i = 0; i < viewList.size(); i++){
        elements << views.value(viewList.at(i))->getElementID();
        elements << views.value(viewList.at(i))->getElementList();
    }

    return elements;

}

QString ColoUiContainer::currentSizeStructure() const{

    QString ans = "";
    QStringList viewList = views.keys();
    for (qint32 i = 0; i < viewList.size(); i++){
        ans = ans + views.value(viewList.at(i)) ->currentSizeStructure() + "\n";
    }
    return ans;

}

void ColoUiContainer::on_transitionTimerTimeout(){
    views.value(viewToInsert)->translateView(activeTransitionDelta,activeTransitionInX);
    views.value(viewToRemove)->translateView(activeTransitionDelta,activeTransitionInX);
    activeTransitionCounter--;
    if (activeTransitionCounter == 0){
        transitionTimer.stop();
        views.value(viewToRemove)->resetDeltasAndZValue();
        views.value(viewToRemove)->removeView(this->scene());        
        views.value(viewToInsert)->resetDeltasAndZValue();
        if (awaitingTransitions.size() > 0){
            awaitingTransitions.removeFirst();
            doNextTransition();
        }
    }
    this->scene()->update();
}

void ColoUiContainer::on_keyboardTranstionTimerTimeout(){
    if (sceneDelta != 0){
        QList<QString> viewNames = views.keys();
        for (qint32 i = 0; i < viewNames.size(); i++){
            views.value(viewNames.at(i))->translateView(sceneDelta,false,1);
        }
    }
    QPointF p = softKeyboard->pos();
    p.setY(p.y() + keyboardDelta);
    softKeyboard->setPos(p);

    keyboardTransitionSteps--;

    if (keyboardTransitionSteps == 0){
        keyboardInTransition = false;
        keyboardTransitionTimer.stop();

        if (keyboardDelta > 0){
            // Should remove the keyboard
            scene()->removeItem(softKeyboard);
        }

        if (awaitingTransitions.size() > 0){
            doNextTransition();
        }
    }
}


void ColoUiContainer::drawUi(){    
    if (uiHasBeenDrawn) return; //UI needs to be drawn only once
    virtualKeyboardInUse = false;
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

void ColoUiContainer::on_coloUiSignal(){
    if (signalManager->getSignalEventInfo().type == ST_KEYBOARD_REQUEST){
        showSoftKeyboard();
    }
    else if (signalManager->getSignalEventInfo().type == ST_KEYBOARD_HIDE){
        hideKeyboard();

        // Sending the text changed signal.
        ColoUiSignalEventInfo info = signalManager->getSignalEventInfo();
        info.type = ST_TEXT_CHANGED;
        signalManager->setSignalInfo(info);

        emit elementSignal();
    }
    else{
        emit elementSignal();
    }
}

void ColoUiContainer::hideKeyboard(){
    keyboardTransitionSteps = 10;
    sceneDelta = -sceneDelta;
    keyboardDelta = -keyboardDelta;
    keyboardInTransition = true;
    keyboardTransitionTimer.setInterval(5);
    keyboardTransitionTimer.start();
}

void ColoUiContainer::showSoftKeyboard(){    

    // Getting the requester
    QString eid = signalManager->getSignalEventInfo().elementID;
    QString viewID;
    QStringList parts = eid.split(".");
    viewID =  parts.first();

    ColoUiView *v = this->getViewByID(viewID);
    if (v == NULL) return;

    ColoUiElement *e = this->getElement(eid);
    if (e == NULL) return;

    if (e->getType() != CUI_LINE_EDIT) return;

    ColoUiLineEdit *t = (ColoUiLineEdit *)e;

    quint16 bottom = t->boundingRect().height() + v->getElementPos(eid).y();

    qreal delta = SCREEN_HEIGHT - softKeyboard->boundingRect().height() - bottom;
    keyboardTransitionSteps = 10;

    if (delta < 0){
        sceneDelta = delta/keyboardTransitionSteps;
    }
    else{
        sceneDelta = 0;
    }

    keyboardDelta = -softKeyboard->boundingRect().height()/keyboardTransitionSteps;
    scene()->addItem(softKeyboard);
    softKeyboard->setPos(0,SCREEN_HEIGHT);
    softKeyboard->setZValue(3);
    softKeyboard->setTextElement(t);
    keyboardInTransition = true;
    keyboardTransitionTimer.setInterval(5);
    keyboardTransitionTimer.start();
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
    this->fitInView(this->scene()->sceneRect(),Qt::IgnoreAspectRatio);
}
