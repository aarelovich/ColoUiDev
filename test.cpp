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
    canvas->setDrawingArea(3,2);

}

void Test::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
}


Test::~Test()
{
    delete ui;
}

void Test::on_actionTest_triggered()
{

}
