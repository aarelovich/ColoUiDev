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

//    if (currentFile.isEmpty()){
//        QString title = WINDOW_TITLE_BASE;
//        this->setWindowTitle(title + "Untitled");
//    }
//    else{
//        QFile file(currentFile);
//        if (file.open(QFile::ReadOnly)){
//            QTextStream stream(&file);
//            ui->ceEditor->setPlainText(stream.readAll());
//            file.close();
//            this->setWindowTitle(WINDOW_TITLE_BASE + currentFile);
//        }
//        else{
//            currentFile = "";
//            QString title = WINDOW_TITLE_BASE;
//            this->setWindowTitle(title + "Untitled");
//        }
//    }

    //  Connections
    ui->lwDocuments->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->lwDocuments,&QListWidget::customContextMenuRequested,this,&ColoUiDesigner::on_documentListContextMenu_requested);
    connect(ui->lwDocuments,&QListWidget::itemDoubleClicked,this,&ColoUiDesigner::on_documentListItem_doubleClicked);

}

ColoUiDesigner::~ColoUiDesigner()
{
    delete ui;
}


void ColoUiDesigner::on_coloUIEvent(){
}

//######################################################### HELPER FUNCTIONS #########################################################

void ColoUiDesigner::log(QString message, QString color){

    ui->teLog->setTextColor(QColor(color));
    ui->teLog->append(message);

}

void ColoUiDesigner::saveSettings(){
    QSettings settings(SETTINGS_FILE,QSettings::IniFormat);
    settings.setValue(SETTINGS_MASTER_FILE,masterFile);
    settings.setValue(SETTINGS_PROJECT_LOCATION,projectLocation);
    settings.setValue(SETTINGS_PROJECT_NAME,projectName);
    settings.setValue(SETTINGS_SPLITTER,ui->splitter->saveGeometry());
    settings.setValue(SETTINGS_SPLITTER_2,ui->splitter_2->saveGeometry());
    settings.setValue(SETTINGS_SPLITTER_3,ui->splitter_3->saveGeometry());
    settings.setValue(SETTINGS_SPLITTER_4,ui->splitter_4->saveGeometry());
    settings.setValue(SETTINGS_WINDOW_SIZE,this->saveGeometry());
    settings.setValue(SETTINGS_SPLITTER_2_STATE,ui->splitter_2->saveState());
    settings.setValue(SETTINGS_SPLITTER_STATE,ui->splitter->saveState());
    settings.setValue(SETTINGS_SPLITTER_3_STATE,ui->splitter_3->saveState());
    settings.setValue(SETTINGS_SPLITTER_4_STATE,ui->splitter_4->saveState());

}

void ColoUiDesigner::loadSettings(){
    QSettings settings(SETTINGS_FILE,QSettings::IniFormat);
    this->restoreGeometry(settings.value(SETTINGS_WINDOW_SIZE).toByteArray());
    ui->splitter_2->restoreGeometry(settings.value(SETTINGS_SPLITTER_2).toByteArray());
    ui->splitter->restoreGeometry(settings.value(SETTINGS_SPLITTER).toByteArray());
    ui->splitter->restoreState(settings.value(SETTINGS_SPLITTER_STATE).toByteArray());
    ui->splitter_2->restoreState(settings.value(SETTINGS_SPLITTER_2_STATE).toByteArray());
    ui->splitter_3->restoreGeometry(settings.value(SETTINGS_SPLITTER_3).toByteArray());
    ui->splitter_3->restoreState(settings.value(SETTINGS_SPLITTER_3_STATE).toByteArray());
    ui->splitter_4->restoreGeometry(settings.value(SETTINGS_SPLITTER_4).toByteArray());
    ui->splitter_4->restoreState(settings.value(SETTINGS_SPLITTER_4_STATE).toByteArray());
    projectLocation = settings.value(SETTINGS_PROJECT_LOCATION).toString();
    masterFile = settings.value(SETTINGS_MASTER_FILE).toString();
    projectName = settings.value(SETTINGS_PROJECT_NAME).toString();
    currentFile = masterFile;

    updateDocumentList();

}

void ColoUiDesigner::updateDocumentList(){

    // Saving any changes.

    QStringList allfiles;
    QDir pdir(projectLocation + "/" + PRJ_SOURCES_DIR);
    QStringList cuiFiles = QStringList() << "*.cui";
    allfiles = pdir.entryList(cuiFiles,QDir::Files);

    ui->lwDocuments->clear();
    for (qint32 i = 0; i < allfiles.size(); i++){
        QString fileName = allfiles.at(i);
        QString comparison = pdir.path() + "/" + fileName;
        fileName.remove(".cui");

        QListWidgetItem *item = new QListWidgetItem(ui->lwDocuments);

        if (comparison == masterFile){
            item->setIcon(QIcon(":/assets/master.png"));
        }

        item->setText(fileName);
        ui->lwDocuments->addItem(item);
    }

    ui->ceEditor->clear();

    if (ui->lwDocuments->count() > 0){
        currentFile = masterFile;
        on_documentListItem_doubleClicked(ui->lwDocuments->item(0));
    }
    else{
        currentFile = "";
    }

    updateTitle();

}

void ColoUiDesigner::styleDocumentList(){

    for (qint32 i = 0; i < ui->lwDocuments->count(); i++){
        QListWidgetItem *item = ui->lwDocuments->item(i);
        item->setBackgroundColor(QColor("#3E00B3"));
        item->setTextColor(QColor("#FFFDF3"));
    }

}

void ColoUiDesigner::updateTitle(){
    QString title = WINDOW_TITLE_BASE;
    QFileInfo info(currentFile);
    title = title + projectName + ": " + info.fileName();
    this->setWindowTitle(title);
}

//######################################################### EVENTS REIMPLEMENTED #########################################################

void ColoUiDesigner::closeEvent(QCloseEvent *e){
    Q_UNUSED(e)
    saveSettings();
}


//######################################################### ACTIONS TRIGGERED #########################################################

void ColoUiDesigner::on_documentListContextMenu_requested(QPoint pos){
    QMenu popup;
    QAction *action = popup.addAction("Set as master file");
    pos = ui->lwDocuments->mapToGlobal(pos);
    connect(action,&QAction::triggered,this,&ColoUiDesigner::on_setFileAsMaster);
    popup.exec(pos);
}

void ColoUiDesigner::on_setFileAsMaster(){
    QString file = ui->lwDocuments->currentItem()->text();
    masterFile = projectLocation + "/";
    masterFile = masterFile + PRJ_SOURCES_DIR;
    masterFile = masterFile + "/" + file + ".cui";
    updateDocumentList();
}

void ColoUiDesigner::on_documentListItem_doubleClicked(QListWidgetItem *item){
    QString filename = "/" + item->text() + ".cui";
    QFile file2read(projectLocation + "/" + PRJ_SOURCES_DIR  + filename);

    if (file2read.open(QFile::ReadOnly)){
        QTextStream stream(&file2read);
        ui->ceEditor->setPlainText(stream.readAll());
        file2read.close();
        currentFile = file2read.fileName();
        styleDocumentList();
        item->setBackgroundColor(QColor("#580265"));
        updateTitle();
    }
    else{
        log("Could not open file " + file2read.fileName() + " for reading","#FF0000");
    }
}

void ColoUiDesigner::on_actionSave_triggered()
{
    if (currentFile.isEmpty()) return;

    QFile file(currentFile);
    if (file.open(QFile::WriteOnly)){
        QTextStream stream(&file);
        stream << ui->ceEditor->toPlainText();
        file.close();
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

void ColoUiDesigner::on_actionOpen_triggered()
{
   QString c = QFileDialog::getExistingDirectory(this,"Search Project Directory",".");
   if (!c.isEmpty()){
       projectLocation = c;
       QDir dir(c);
       QString temp = dir.path();
       QStringList parts = temp.split("/");
       projectName = parts.last();
       masterFile = "";
       currentFile = "";
       updateDocumentList();
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

    on_actionSave_triggered();
    if (masterFile.isEmpty()){
        log("Master file must be set","#FF0000");
        return;
    }

    QString assetOuput = projectLocation + PRJ_ASSESTS_DIR;
    assetOuput = assetOuput + "/";
    assetOuput = assetOuput + PRJ_PROC_CUI_FILE;

    ColoUiCreator parser;
    parser.createUi(masterFile,assetOuput,previewWindow->coloUiContainter());
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

    if (!fname.endsWith(".cui")){
        fname = fname + ".cui";
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
        log("Could not create project directory " + pdir,"#FF0000");
        return;
    }

    QStringList dirlist = QStringList() << PRJ_SOURCES_DIR << PRJ_ASSESTS_DIR << PRJ_PROJECTGEN_DIR;
    for (qint32 i = 0; i < dirlist.size(); i++){
        QString adir = dirlist.at(i);
        if (!location.mkpath(pdir+"/" +adir)){
            log("Could not create project directory " + pdir + "/" + adir,"#FF0000");
            return;
        }
    }

    // Creating first, master file
    QString masterfile = loc+"/"+pdir+"/" + PRJ_SOURCES_DIR;
    masterfile = masterfile + "/" + fname;
    QFile master(masterfile);

    if (!master.open(QFile::WriteOnly)){
        log("Could not create project master file " + master.fileName(),"#FF0000");
        return;
    }

    QTextStream writer(&master);
    writer << "% Automatically generated Master File for Project\n\n" + pdir;
    writer << "DRAW_AREA\n";
    writer << "   width    | 1000\n";
    writer << "   height   | 1000\n";
    writer << "DONE\n";

    master.close();

    projectLocation = loc + "/" + pdir;
    projectName = pdir;
    masterFile = master.fileName();

    updateDocumentList();

}

void ColoUiDesigner::on_actionAdd_file_triggered()
{
    on_actionSave_triggered();

    QString name = QInputDialog::getText(this,"New file","Name for new file (extension will be added automatically)",QLineEdit::Normal,"cui_file");
    if (name.isEmpty()) return;

    QFile file(projectLocation + "/" + PRJ_SOURCES_DIR + "/" + name + ".cui");
    if (!file.open(QFile::WriteOnly)){
        log("Could not create new file " + file.fileName(),"#FF0000");
        return;
    }

    QTextStream writer(&file);
    writer << "% File for project " + projectName;
    file.close();

    // Adding the item to the list and selecting it.
    QListWidgetItem *item = new QListWidgetItem(ui->lwDocuments);
    item->setText(name);
    ui->lwDocuments->addItem(item);
    on_documentListItem_doubleClicked(item);

}

void ColoUiDesigner::on_actionRemove_file_triggered()
{
    QMessageBox::Button b =  QMessageBox::question(this,"File removal","Are you sure you want to remove the current file?\nThe file will be deleted from disk as well");
    if (b == QMessageBox::Ok){
        ui->ceEditor->clear();

        QFile f(currentFile);
        if (!f.remove()){
            log("Problem deleting curret file " + currentFile,"#FF0000");
        }

        if (currentFile == masterFile){
            masterFile = "";
        }
        currentFile = "";
        updateDocumentList();

    }
}
