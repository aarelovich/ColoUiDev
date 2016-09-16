#include "test.h"
#include "ui_test.h"

Test::Test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test)
{
    ui->setupUi(this);
    canvas = new ColoUiContainer();
    ui->global->addWidget(canvas);
    canvas->setDrawDrawAreaRect(true);
    canvas->setDrawingArea(180,120);

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
    canvas->createView(viewID,20,20,100,100);
    ColoUiView *view = canvas->getViewByID(viewID);
    ColoUiSignalManager *sm = canvas->getSignalManager();

    ColoUiElementConfig c;
    c.borderColor = QColor(Qt::yellow);
    c.borderWidth = 0;
    c.font = QFont("Deja Vu Sans",12);
    c.backgroundColor = QColor(Qt::red);
    c.textColor = QColor(Qt::black);
    c.alternativeTextColor = c.textColor.lighter();
    c.alternativeBackgroundColor = c.backgroundColor.lighter();
    c.shape = SHAPE_ELLIPSE;
    c.width = 80;
    c.height = 20;
    c.x = 10;
    c.y = 5;

    // Adding the first button
//    c.text = "Button 1";
//    view->createElement(CUI_BUTTON,"Button1",c);

//    c.text = "Button 2";
//    c.y = 30;
//    c.shape = SHAPE_ROUNDED_RECT;
//    view->createElement(CUI_BUTTON,"Button2",c);

    c.text = "Button 3";
    c.y = 55;
    c.shape = SHAPE_RECT;
    view->createElement(CUI_BUTTON,"Button3",c,sm);

    canvas->drawUi();
}
