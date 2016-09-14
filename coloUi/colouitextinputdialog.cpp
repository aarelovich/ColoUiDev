#include "colouitextinputdialog.h"

ColoUiTextInputDialog::ColoUiTextInputDialog(QWidget *parent) : QDialog(parent)
{
    this->setBaseSize(SCREEN_WIDTH,SCREEN_HEIGHT*0.3);
    QPushButton *ok = new QPushButton(this);
    QPushButton *cancel = new QPushButton(this);
    QHBoxLayout *layout = new QHBoxLayout();
    QVBoxLayout *global = new QVBoxLayout();
    ok->setText("OK");
    cancel->setText("Cancel");
    layout->addWidget(cancel);
    layout->addStretch();
    layout->addWidget(ok);
    input = new QLineEdit(this);
    global->addWidget(input);
    global->addLayout(layout);
    this->setLayout(global);
    connect(ok,&QPushButton::clicked,this,&ColoUiTextInputDialog::accept);
    connect(cancel,&QPushButton::clicked,this,&ColoUiTextInputDialog::reject);
}
