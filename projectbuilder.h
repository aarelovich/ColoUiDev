#ifndef PROJECTBUILDER_H
#define PROJECTBUILDER_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

namespace Ui {
class ProjectBuilder;
}

class ProjectBuilder : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectBuilder(QWidget *parent = 0);
    ~ProjectBuilder();

    void setProjectName(QString pname);
    void setColoUiFolderLocation(QString floc);
    void setupBuild(QString uifile);

    QString getMainWindowClassName() const;
    QString getColoUiFolderLocation() const;
    QString getProjectName() const;

private slots:
    void on_pbSearch_clicked();

    void on_pbBuild_clicked();

    void on_pbCancel_clicked();

    void on_pbSearchPLoc_clicked();

private:

    const QString KEY_CLASS_NAME = QString("WINDOW_CLASS_NAME");
    const QString KEY_CLASS_NAME_LOWER = QString("WINDOW_CLASS_NAME_LOWER");
    const QString KEY_CLASS_NAME_UPPER = QString("WINDOW_CLASS_NAME_UPPER");
    const QString KEY_COLOUI_LOCATION = QString("COLOUI_PATH");
    const QString KEY_PROJECT_NAME = QString("PROJECT_NAME");

    struct SearchAndReplace{
        QString search;
        QString replace;
    };

    Ui::ProjectBuilder *ui;

    void showError(QString msg);

    QString uiFile;

    bool genFile(QVector<SearchAndReplace> strs, QString source, QString dest);

};

#endif // PROJECTBUILDER_H
