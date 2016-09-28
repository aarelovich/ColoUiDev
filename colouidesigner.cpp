#include "colouidesigner.h"
#include "ui_colouidesigner.h"

ColoUiDesigner::ColoUiDesigner(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ColoUiDesigner)
{
    ui->setupUi(this);

    previewWindow = new PreviewWidget();
    previewWindow->hide();

    // Initializing Code editor Style

    // Font
    int idb  = QFontDatabase::addApplicationFont(":/assets/unispace_b.ttf");
    QFontDatabase::addApplicationFont(":/assets/unispace_reg.ttf");
    QString family = QString(QFontDatabase::applicationFontFamilies(idb).at(0));
    qint32 stdFontSize = 9;
    QFont font(family,stdFontSize);
    ui->ceEditor->setFont(font);

    // Background and main text
    QPalette p = ui->ceEditor->palette();
    p.setColor(QPalette::Base,QColor(Qt::black));
    p.setColor(QPalette::Text,QColor("#0ffdd6"));
    ui->ceEditor->setPalette(p);

    // Highlight color
    ui->ceEditor->setHighlightLineColor(QColor("#444444"));

    // The words.
    QTextCharFormat format;
    format.setFont(QFont(family,stdFontSize));
    format.setForeground(QBrush("#ea00ff"));
    format.setFontWeight(QFont::Bold);
    ui->ceEditor->syntaxHighlighter()->setWordGroupFormat(ColoUiDeclarations,format);

    // The properties
    format.setFont(QFont(family,stdFontSize));
    format.setForeground(QColor("#ff0051"));
    format.setFontWeight(QFont::Normal);
    ui->ceEditor->syntaxHighlighter()->setWordGroupFormat(ColoUiProperties,format);

    // The parameter names.
    QStringList paramNames = ColoUiParameters.keys();
    format.setForeground(QColor("#0252ff"));
    ui->ceEditor->syntaxHighlighter()->setWordGroupFormat(paramNames,format);

    // Numbers
    format.setForeground(QColor("#28ff02"));
    ui->ceEditor->syntaxHighlighter()->setNumberFormat(format);

    // The comments
    format.setFontItalic(true);
    format.setForeground(QColor("#FFFFFF"));
    ui->ceEditor->syntaxHighlighter()->setLineCommentFormat("%",format);


    // Loading settings
    loadSettings();
    if (currentFile.isEmpty()){
        QString title = WINDOW_TITLE_BASE;
        this->setWindowTitle(title + "Untitled");
    }
    else{
        QFile file(currentFile);
        if (file.open(QFile::ReadOnly)){
            QTextStream stream(&file);
            ui->ceEditor->setPlainText(stream.readAll());
            file.close();
            this->setWindowTitle(WINDOW_TITLE_BASE + currentFile);
        }
        else{
            currentFile = "";
            QString title = WINDOW_TITLE_BASE;
            this->setWindowTitle(title + "Untitled");
        }
    }

}

void ColoUiDesigner::on_coloUIEvent(){
}

void ColoUiDesigner::saveSettings(){
    QSettings settings(SETTINGS_FILE,QSettings::IniFormat);
    //settings.setValue(SETTINGS_LAST_FILE,currentFile);
    settings.setValue(SETTINGS_SPLITTER,ui->splitter->saveGeometry());
    settings.setValue(SETTINGS_SPLITTER_2,ui->splitter_2->saveGeometry());
    settings.setValue(SETTINGS_SPLITTER_3,ui->splitter_3->saveGeometry());
    settings.setValue(SETTINGS_WINDOW_GEOMETRY,this->saveGeometry());
    settings.setValue(SETTINGS_SPLITTER_2_STATE,ui->splitter_2->saveState());
    settings.setValue(SETTINGS_SPLITTER_STATE,ui->splitter->saveState());    
    settings.setValue(SETTINGS_SPLITTER_3_STATE,ui->splitter_3->saveState());

}

void ColoUiDesigner::loadSettings(){
    QSettings settings(SETTINGS_FILE,QSettings::IniFormat);
    //currentFile = settings.value(SETTINGS_LAST_FILE,"").toString();
    this->restoreGeometry(settings.value(SETTINGS_WINDOW_GEOMETRY).toByteArray());
    ui->splitter_2->restoreGeometry(settings.value(SETTINGS_SPLITTER_2).toByteArray());
    ui->splitter->restoreGeometry(settings.value(SETTINGS_SPLITTER).toByteArray());
    ui->splitter->restoreState(settings.value(SETTINGS_SPLITTER_STATE).toByteArray());
    ui->splitter_2->restoreState(settings.value(SETTINGS_SPLITTER_2_STATE).toByteArray());
    ui->splitter_3->restoreGeometry(settings.value(SETTINGS_SPLITTER_3).toByteArray());
    ui->splitter_3->restoreState(settings.value(SETTINGS_SPLITTER_3_STATE).toByteArray());
}

ColoUiDesigner::~ColoUiDesigner()
{
    delete ui;
}

void ColoUiDesigner::closeEvent(QCloseEvent *e){
    Q_UNUSED(e)
    saveSettings();
}

void ColoUiDesigner::on_actionSave_triggered()
{
    if (currentFile.isEmpty()){
        currentFile = QFileDialog::getSaveFileName(this,"Save CUI file",".","ColoUiDev Files (*.cui)");
    }

    if (currentFile.isEmpty()) return;

    QFile file(currentFile);
    if (file.open(QFile::WriteOnly)){
        QTextStream stream(&file);
        stream << ui->ceEditor->toPlainText();
        file.close();
        this->setWindowTitle(WINDOW_TITLE_BASE + currentFile);
    }
}

void ColoUiDesigner::on_actionInsert_Color_triggered()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid()){
        QTextCursor cursor = ui->ceEditor->textCursor();
        cursor.insertText(color.name());
        ui->ceEditor->setTextCursor(cursor);
    }
}

void ColoUiDesigner::log(QString message, QString color){

    ui->teLog->setTextColor(QColor(color));
    ui->teLog->append(message);

}

void ColoUiDesigner::on_actionOpen_triggered()
{
    QString c = QFileDialog::getOpenFileName(this,"Load CUI File",".","ColoUiDev Files (*.cui)");
    if (c.isEmpty()) return;

    currentFile = c;
    QFile file(currentFile);
    if (file.open(QFile::ReadOnly)){
        QTextStream stream(&file);
        ui->ceEditor->setPlainText(stream.readAll());
        file.close();
        this->setWindowTitle(WINDOW_TITLE_BASE + currentFile);
    }
    else{
        currentFile = "";
        QString title = WINDOW_TITLE_BASE;
        this->setWindowTitle(title + "Untitled");
    }

}

void ColoUiDesigner::on_actionIndent_triggered()
{
    QTextCursor cursor = ui->ceEditor->textCursor();
    QString text = cursor.selectedText();
    QString tab = "   ";
    QChar sep = QChar(0x2029);
    QStringList lines = text.split(sep,QString::KeepEmptyParts);
    for (qint32 i = 0; i < lines.size(); i++){
        lines[i] = tab + lines.at(i);
    }
    text = lines.join(sep);
    cursor.removeSelectedText();
    cursor.insertText(text);

}


void ColoUiDesigner::on_actionUnindent_triggered()
{
    QTextCursor cursor = ui->ceEditor->textCursor();
    QString text = cursor.selectedText();
    QChar sep = QChar(0x2029);
    QStringList lines = text.split(sep,QString::KeepEmptyParts);
    for (qint32 i = 0; i < lines.size(); i++){
        QString line = lines.at(i);
        for (qint32 j = 0; j < 3; j++){
           if (!line.isEmpty()){
               if (line.at(i) == ' '){
                   line.remove(0,1);
               }
           }
        }
        lines[i] = line;
    }
    text = lines.join(sep);
    cursor.removeSelectedText();
    cursor.insertText(text);
}

void ColoUiDesigner::on_actionPreview_triggered()
{
    ColoUiCreator parser;
    parser.createUi(currentFile,previewWindow->coloUiContainter());
    CreatorError ce = parser.getError();
    if (!ce.error.isEmpty()){
        log(ce.error + ". Line " + QString::number(ce.line),"#FF0000");
    }
    else{
        log("All good!!!","#00FF00");
    }
    previewWindow->show();
    previewWindow->fillTransitionComboBox();
}

void ColoUiDesigner::on_actionClean_triggered()
{
    ui->teLog->clear();
}

void ColoUiDesigner::on_actionCreate_Project_triggered()
{
    ProjectCreation p(this);
    if (!p.exec()) return;

    QString loc = p.getLocation();
    QString fname = p.getMasterFile();
    QString pdir = p.getProjectName();

    if (loc.isEmpty()){
        log("Empty project location","#FF0000");
        return;
    }

    if (fname.isEmpty()){
        log("Empyt name for master file","#FF0000");
        return;

    }

    if (pdir.isEmpty()){
        log("Empty project name","#FF0000");
        return;
    }

    if (QDir(loc+"/"+pdir).exists()){
        log("Project location allready contains a directory named " + pdir,"#FF0000");
        return;
    }

    QDir location(loc);
    if (!location.mkdir(pdir)){
        log("Project location allready contains a directory named " + pdir,"#FF0000");
        return;
    }


}
