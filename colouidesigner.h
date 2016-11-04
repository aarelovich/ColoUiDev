#ifndef TEST_H
#define TEST_H

#include <QMainWindow>
#include <QSettings>
#include <QFileDialog>
#include <QTextStream>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QFileInfo>
#include <QMenu>

#include "coloUi/colouicreator.h"
#include "previewwidget.h"
#include "codeeditor.h"
#include "projectcreation.h"
#include "projectbuilder.h"

#define    SETTINGS_FILE              "settings.ini"
#define    SETTINGS_MASTER_FILE       "master_file"
#define    SETTINGS_PROJECT_LOCATION  "project_directory"
#define    SETTINGS_PROJECT_NAME      "project_name"
#define    SETTINGS_COLOUI_LOC        "coloui_location"
#define    SETTINGS_LAST_PRJ_LOC      "project_build_last_location"
#define    SETTINGS_SPLITTER          "splitter"
#define    SETTINGS_SPLITTER_2        "splitter2"
#define    SETTINGS_SPLITTER_3        "splitter3"
#define    SETTINGS_SPLITTER_4        "splitter4"
#define    SETTINGS_SPLITTER_STATE    "splitter_state"
#define    SETTINGS_SPLITTER_2_STATE  "splitter2_state"
#define    SETTINGS_SPLITTER_3_STATE  "splitter3_state"
#define    SETTINGS_SPLITTER_4_STATE  "splitter4_state"
#define    SETTINGS_WINDOW_SIZE       "windowSize"
#define    SETTINGS_ASSETS_FOLDER     "assetsFolder"

// Used for accessing the file and line for config colors and constants
#define    ROLE_NAME                  1532
#define    ROLE_LINE                  1533

namespace Ui {
class ColoUiDesigner;
}

class ColoUiDesigner : public QMainWindow
{
    Q_OBJECT

public:
    explicit ColoUiDesigner(QWidget *parent = 0);
    ~ColoUiDesigner();

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void on_coloUIEvent();

    void on_actionSave_triggered();

    void on_actionInsert_Color_triggered();

    void on_actionOpen_triggered();

    void on_actionIndent_triggered();

    void on_actionUnindent_triggered();

    void on_actionPreview_triggered();

    void on_actionClean_triggered();

    void on_actionCreate_Project_triggered();

    void on_actionAdd_file_triggered();

    void on_actionRemove_file_triggered();

    void on_setFileAsMaster();

    void on_documentListContextMenu_requested(QPoint pos);

    void on_documentListItem_doubleClicked(QListWidgetItem *item);

    void on_definitionsListItem_doubleClicked(QListWidgetItem *item);

    void on_definitionsListContextMenu_requested(QPoint pos);

    void on_definitionsListShowDef_triggered();

    void on_actionBuildQtProject_triggered();

    void on_actionUpdate_triggered();

    void on_actionUncomment_triggered();

    void on_actionInsert_Image_triggered();

    void on_actionSize_structure_triggered();

    void on_keywordListItem_doubleClicked(QListWidgetItem *item);

    void on_actionInsert_normalized_space_triggered();

private:

    const QString WINDOW_TITLE_BASE = QString("ColoUiDesigner v1.5 - ");

    const qint32 TAB_SIZE = 3;
    const QString TAB_STRING = QString("   ");

    Ui::ColoUiDesigner *ui;
    PreviewWidget *previewWindow;

    QString projectLocation;
    QString projectName;
    QString masterFile;
    QString currentFile;
    QString coloUiSrcLocation;
    QString joinedUiFile;
    QString pbuildLastLocation;
    QString assetsFolder;
    QStringList assetsFiles;
    qint32 normSpace;
    bool lastParseWasSucessFull;

    void saveSettings();
    void loadSettings();

    void log(QString message, QString color = "#FFFFFF");
    void updateDocumentList();
    void updateTitle();

    void styleDocumentList();
};

#endif // TEST_H
