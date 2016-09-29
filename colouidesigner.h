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

#define    SETTINGS_FILE              "settings.ini"
#define    SETTINGS_MASTER_FILE       "master_file"
#define    SETTINGS_PROJECT_LOCATION  "project_directory"
#define    SETTINGS_PROJECT_NAME      "project_name"
#define    SETTINGS_SPLITTER          "splitter"
#define    SETTINGS_SPLITTER_2        "splitter2"
#define    SETTINGS_SPLITTER_3        "splitter3"
#define    SETTINGS_SPLITTER_4        "splitter4"
#define    SETTINGS_SPLITTER_STATE    "splitter_state"
#define    SETTINGS_SPLITTER_2_STATE  "splitter2_state"
#define    SETTINGS_SPLITTER_3_STATE  "splitter3_state"
#define    SETTINGS_SPLITTER_4_STATE  "splitter4_state"
#define    SETTINGS_WINDOW_SIZE       "windowSize"

#define    PRJ_SOURCES_DIR            "src"
#define    PRJ_ASSESTS_DIR            "assets"
#define    PRJ_PROJECTGEN_DIR         "prjgen"
#define    PRJ_PROC_CUI_FILE          "ui_descriptor.cui"

#define    WINDOW_TITLE_BASE  "ColoUiDesigner v0.1 - "


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


private:
    Ui::ColoUiDesigner *ui;
    PreviewWidget *previewWindow;

    QString projectLocation;
    QString projectName;
    QString masterFile;
    QString currentFile;

    void saveSettings();
    void loadSettings();

    void log(QString message, QString color = "#FFFFFF");
    void updateDocumentList();
    void updateTitle();

    void styleDocumentList();
};

#endif // TEST_H
