#include "test.h"
#include "ui_test.h"

Test::Test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test)
{
    ui->setupUi(this);
    canvas = new ColoUiContainer();
    ui->global->addWidget(canvas);
    canvas->setDrawingArea(3600,2400);
    canvas->setForceNoScrollBars(true);
    connect(canvas->getSignalManager(),&ColoUiSignalManager::signalTriggered,this,&Test::on_coloUIEvent);

}

void Test::on_coloUIEvent(){
    qDebug() << "Signal From" << canvas->getSignalManager()->getSignalEventInfo().elementID;
}

Test::~Test()
{
    delete ui;
}

void Test::on_actionTest_triggered()
{

    QString viewID = "FirstView";
    qDebug() << canvas->createView(viewID,10,20,50,50,true);
    ColoUiView *view = canvas->getViewByID(viewID);
    ColoUiSignalManager *sm = canvas->getSignalManager();

    ColoUiElementConfig c = ColoUi::standardElementConfig();
    c.borderColor = QColor(Qt::yellow);
    c.borderWidth = 0;
    c.fontSize = 60;
    c.backgroundColor = QColor(Qt::red);
    c.textColor = QColor(Qt::black);
    c.alternativeTextColor = QColor(Qt::gray);
    c.alternativeBackgroundColor = c.backgroundColor.lighter();
    c.shape = SHAPE_ELLIPSE;
    c.width = 50;
    c.height = 20;
    c.x = 0;
    c.y = 0;
    c.iconPosition = ICON_LEFT;
    c.iconPath = ":/assets/gundam_icon.png";

    c.text = "Button";
    c.shape = SHAPE_RECT;
    //c.yOffset = view->getViewRect().height()*-.02;
    qDebug() << view->createElement(CUI_BUTTON,"Button1",c,sm,true);


    QString viewID2 = "SecondView";
    qDebug() << canvas->createView(viewID2,70,20,20,50,true);
    view = canvas->getViewByID(viewID2);

    c.text = "RoundRect";
    c.fontSize = 24;
    c.shape = SHAPE_ROUNDED_RECT;
    c.roundeRectRadious = 50;
    c.y = 21;
    qDebug() << view->createElement(CUI_BUTTON,"Button3",c,sm,true);

    c.text = "Ellipse";
    c.shape = SHAPE_ELLIPSE;
    c.y = 42;
    c.height = c.width;
    qDebug() << view->createElement(CUI_BUTTON,"Button2",c,sm,true);

    // Creating secondary view
    QString tviewID = "TransitionView";
    qDebug() << canvas->createView(tviewID,10,20,50,50,true);
    ColoUiView *tview = canvas->getViewByID(tviewID);
    ColoUiElementConfig temp = ColoUi::standardElementConfig();
    temp.iconPath = ":/assets/gundam_icon.png";
    temp.backgroundColor = QColor(Qt::green);
    temp.x = 0;
    temp.y = 0;
    temp.width = 100;
    temp.height = 100;
    tview->createElement(CUI_BUTTON,"BigAssButton",temp,sm,true);

    // Adding a transition
    ColoUiTransition t = ColoUi::standardTransition();
    t.viewA = tviewID;
    t.viewB = viewID;
    t.type = TRANSITION_LEFT;
    t.transitionLengthInMS = 1000;
    canvas->addTransition(t);

    canvas->drawUi();
}

void Test::on_actionMovie_triggered()
{
    canvas->startTranstion("FirstView","TransitionView");
}
