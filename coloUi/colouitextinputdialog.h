#ifndef COLOUITEXTINPUTDIALOG_H
#define COLOUITEXTINPUTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include "colouibase.h"

class ColoUiTextInputDialog : public QDialog, ColoUiBase
{
    Q_OBJECT
public:
    explicit ColoUiTextInputDialog(QWidget *parent = 0);

    QString getInput() const {return input->text();}
    void setInput(QString str){ input->setText(str); }

private:
    QLineEdit *input;


};

#endif // COLOUITEXTINPUTDIALOG_H
