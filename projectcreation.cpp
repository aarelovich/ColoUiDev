#include "projectcreation.h"
#include "ui_projectcreation.h"

ProjectCreation::ProjectCreation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectCreation)
{
    ui->setupUi(this);
}

ProjectCreation::~ProjectCreation()
{
    delete ui;
}

QString ProjectCreation::getProjectName() const {
    return ui->leProjectName->text();
}

QString ProjectCreation::getLocation() const {
    return ui->leProjectLocation->text();
}

QString ProjectCreation::getMasterFile() const{
    return ui->leMasterFile->text();
}

void ProjectCreation::on_pushButton_clicked()
{
    ui->leProjectLocation->setText(QFileDialog::getExistingDirectory(this,"Select project location","."));
}
