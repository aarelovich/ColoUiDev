#include "previewwidget.h"
#include "ui_previewwidget.h"

PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewWidget)
{
    ui->setupUi(this);
}

ColoUiContainer* PreviewWidget::coloUiContainter() const{
    return ui->graphicsView;
}


PreviewWidget::~PreviewWidget()
{
    delete ui;
}

void PreviewWidget::fillTransitionComboBox(){
    QVector<ColoUiConfiguration> t = ui->graphicsView->getTransitions();

    ui->cbTranstion->clear();
    if (t.isEmpty()){
        ui->cbTranstion->setEnabled(false);
        ui->pbTranstion->setEnabled(false);
        return;
    }
    ui->cbTranstion->setEnabled(true);
    ui->pbTranstion->setEnabled(true);

    for (qint32 i = 0; i < t.size(); i++){

        QStringList views;
        QString tran = t.at(i).getString(CPR_TRANSITION_VIEW_A) + " <--> " + t.at(i).getString(CPR_TRANSITION_VIEW_B);
        views << t.at(i).getString(CPR_TRANSITION_VIEW_A) << t.at(i).getString(CPR_TRANSITION_VIEW_B);
        ui->cbTranstion->addItem(tran,views);

    }
}

void PreviewWidget::on_pbTranstion_clicked()
{
    if (ui->cbTranstion->currentIndex() > -1){
        QStringList views = ui->cbTranstion->currentData().toStringList();
        ui->graphicsView->startTranstion(views.first(),views.last());
    }
}
