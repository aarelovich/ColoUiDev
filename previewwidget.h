#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include "coloUi/colouicontainer.h"

namespace Ui {
class PreviewWidget;
}

class PreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewWidget(QWidget *parent = 0);
    ~PreviewWidget();

    ColoUiContainer* coloUiContainter() const;
    void fillTransitionComboBox();

private slots:
    void on_pbTranstion_clicked();

private:
    Ui::PreviewWidget *ui;
};

#endif // PREVIEWWIDGET_H
