#ifndef TEST_H
#define TEST_H

#include <QMainWindow>
#include <QSettings>
#include <QFileDialog>
#include <QTextStream>
#include <QColorDialog>
#include <QVBoxLayout>

#include "coloUi/colouicreator.h"
#include "previewwidget.h"
#include "codeeditor.h"

#define    SETTINGS_FILE              "settings.ini"
#define    SETTINGS_LAST_FILE         "last_file"
#define    SETTINGS_SPLITTER          "splitter"
#define    SETTINGS_SPLITTER_2        "splitter2"
#define    SETTINGS_SPLITTER_STATE    "splitter_state"
#define    SETTINGS_SPLITTER_2_STATE  "splitter2_state"
#define    SETTINGS_WINDOW_GEOMETRY   "windowGeometry"

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

private:
    Ui::ColoUiDesigner *ui;
    PreviewWidget *previewWindow;
    QString currentFile;

    void saveSettings();
    void loadSettings();

    void log(QString message, QString color = "#FFFFFF");
};

#endif // TEST_H
