#ifndef PROJECTCREATION_H
#define PROJECTCREATION_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class ProjectCreation;
}

class ProjectCreation : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectCreation(QWidget *parent = 0);
    QString getLocation() const;
    QString getProjectName() const;
    QString getMasterFile() const;
    ~ProjectCreation();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ProjectCreation *ui;
};

#endif // PROJECTCREATION_H
