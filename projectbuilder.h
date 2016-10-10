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

    void setupBuild(QString uifile, QStringList elements, QString pname, QString floc, QString lastPLoc, QStringList assets);
    void setupUpdate(QString uifile, QString ploc, QStringList elements, QStringList assets);
    bool generateElementsFile(QString filename, QStringList *if_else_chain = nullptr);

    QString getMainWindowClassName() const;
    QString getColoUiFolderLocation() const;
    QString getProjectName() const;
    QString getProjectBuildLocation() const;
    QString getElementsFile() const {return elementsFile;}
    QString getFinalUiFile() const {return finalUiFile;}

private slots:
    void on_pbSearch_clicked();

    void on_pbBuild_clicked();

    void on_pbCancel_clicked();

    void on_pbSearchPLoc_clicked();

    void on_pbUpdate_clicked();

private:

    Ui::ProjectBuilder *ui;

    // Variables.
    QString uiFile;
    QString elementsFile;
    QString finalUiFile;
    QStringList uiElements;
    QStringList assetsFiles;

    // Replace strings
    const QString KEY_CLASS_NAME = QString("WINDOW_CLASS_NAME");
    const QString KEY_CLASS_NAME_LOWER = QString("WINDOW_CLASS_NAME_LOWER");
    const QString KEY_CLASS_NAME_UPPER = QString("WINDOW_CLASS_NAME_UPPER");
    const QString KEY_COLOUI_LOCATION = QString("COLOUI_PATH");
    const QString KEY_PROJECT_NAME = QString("PROJECT_NAME");
    const QString KEY_IF_ELSE_CHAIN = QString("IF_ELSE_SIGNAL_CHAIN");
    const QString KEY_QRC_ASSETS = QString("COLOUI_ASSETS_ENTRY");

    struct SearchAndReplace{
        QString search;
        QString replace;
    };

    // Helper functions
    void showError(QString msg);
    bool genFile(QVector<SearchAndReplace> strs, QString source, QString dest);
    SearchAndReplace updateQRC(QString ploc);

};

#endif // PROJECTBUILDER_H
