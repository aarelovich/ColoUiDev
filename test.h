#ifndef TEST_H
#define TEST_H

#include <QMainWindow>
#include "coloUi/colouicontainer.h"

namespace Ui {
class Test;
}

class Test : public QMainWindow
{
    Q_OBJECT

public:
    explicit Test(QWidget *parent = 0);
    ~Test();


private slots:
    void on_coloUIEvent();
    void on_actionTest_triggered();


    void on_actionMovie_triggered();

private:
    Ui::Test *ui;
    ColoUiContainer *canvas;
};

#endif // TEST_H
