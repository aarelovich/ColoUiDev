#include "projectbuilder.h"
#include "ui_projectbuilder.h"

ProjectBuilder::ProjectBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectBuilder)
{
    ui->setupUi(this);
    showError("");
}

void ProjectBuilder::setupBuild(QString uifile,
                                QStringList elements,
                                QString pname,
                                QString floc,
                                QString lastPLoc,
                                QStringList assets){
    uiFile = uifile;
    uiElements = elements;
    ui->leProjectName->setText(pname);
    ui->leColoUiFolderLocation->setText(floc);
    ui->leProjectLocation->setText(lastPLoc);
    assetsFiles = assets;

    // Hiding what doesn't go
    ui->pbUpdate->setVisible(false);
    ui->pbSearch->setEnabled(true);
    ui->pbBuild->setVisible(true);
    ui->leColoUiFolderLocation->setEnabled(true);
    ui->leMainWindow->setEnabled(true);
    ui->leProjectName->setEnabled(true);

}

void ProjectBuilder::setupUpdate(QString uifile, QString ploc, QStringList elements, QStringList assets){

    uiElements = elements;
    uiFile = uifile;
    assetsFiles = assets;

    ui->leProjectLocation->setText(ploc);

    // Hiding what doesn't go
    ui->pbSearch->setEnabled(false);
    ui->pbBuild->setVisible(false);
    ui->leColoUiFolderLocation->setEnabled(false);
    ui->leMainWindow->setEnabled(false);
    ui->leProjectName->setEnabled(false);

}

QString ProjectBuilder::getProjectName() const{
    return ui->leProjectName->text();
}

QString ProjectBuilder::getColoUiFolderLocation() const{
    return ui->leColoUiFolderLocation->text();
}

QString ProjectBuilder::getMainWindowClassName() const{
    return ui->leMainWindow->text();
}

QString ProjectBuilder::getProjectBuildLocation() const{
    return ui->leProjectLocation->text();
}


ProjectBuilder::~ProjectBuilder()
{
    delete ui;
}

void ProjectBuilder::on_pbSearch_clicked()
{
    ui->leColoUiFolderLocation->setText(QFileDialog::getExistingDirectory(this,
                                                                          "ColoUi Folder (Location of all sources)",
                                                                          ui->leColoUiFolderLocation->text()));
}

void ProjectBuilder::on_pbCancel_clicked()
{
    this->done(1);
}

void ProjectBuilder::on_pbSearchPLoc_clicked()
{
    ui->leProjectLocation->setText(QFileDialog::getExistingDirectory(this,
                                                                     "Qt Creator Project Location",
                                                                      QDir::currentPath()));

}

void ProjectBuilder::showError(QString msg){
    if (msg.isEmpty()){
        ui->lMessage->setVisible(false);
        return;
    }

    ui->lMessage->setVisible(true);
    QString html = "<span style = 'font-weight: bold; color: #FFFFFF; background-color:#FF0000'>";
    html = html + msg + "</span>";
    ui->lMessage->setText(html);

}

void ProjectBuilder::on_pbBuild_clicked()
{
    QString ploc = ui->leProjectLocation->text();
    QString pname = ui->leProjectName->text();
    QString colouisrc = ui->leColoUiFolderLocation->text();
    QString mainw = ui->leMainWindow->text();

    showError("");

    if (ploc.isEmpty()){
        showError("Project location cannot be empty");
        return;
    }
    QDir loc(ploc);
    if (!loc.exists()){
        showError("Project location does not exist");
        return;
    }

    if (pname.isEmpty()){
        showError("Project name cannot be empty");
        return;
    }
    else{
        if (pname.contains(" ")){
            showError("Project name cannot contain spaces");
            return;
        }
    }

    if (colouisrc.isEmpty()){
        showError("Location of ColoUi Source Files cannot be empty");
        return;
    }
    QDir srcdir(colouisrc);
    if (!srcdir.exists()){
        showError("ColoUi Source Files directory does not exist");
        return;
    }

    if (mainw.isEmpty()){
        showError("Main window class name cannto be empty");
        return;
    }

    // Creating the project directory
    if (!QDir(ploc+"/"+pname).exists()){
        if (!loc.mkdir(pname)){
            showError("Could not create dir " + pname + " in folder " + loc.absolutePath());
            return;
        }
    }

    if (!QFile(uiFile).exists()){
        showError("The compiled UI file does not exist " + uiFile);
        return;
    }

    // Creating the assets directory
    QDir pdir(ploc + "/" + pname);
    if (!QDir(pdir.absolutePath() + "/assets").exists()){
        if (!pdir.mkdir("assets")){
            showError("Could not create the assets directory in " + pdir.absolutePath());
            return;
        }
    }

    // Copying the ui file
    QFile duifile(pdir.absolutePath() + "/assets/ui_descriptor.cui");
    if (duifile.exists()){
        duifile.remove();
    }

    if (!QFile::copy(uiFile,duifile.fileName())){
        showError("Could not copy ui file: " + uiFile + " to " + duifile.fileName());
        return;
    }

    finalUiFile = duifile.fileName();

    // Strings to replace
    QVector<SearchAndReplace> strs;
    SearchAndReplace sr;

    sr.search = "<!**" + KEY_CLASS_NAME + "**!>";
    sr.replace = mainw;
    strs << sr;

    sr.search = "<!**" + KEY_CLASS_NAME_LOWER + "**!>";
    sr.replace = mainw.toLower();
    strs << sr;

    sr.search = "<!**" + KEY_CLASS_NAME_UPPER + "**!>";
    sr.replace = mainw.toUpper();
    strs << sr;

    sr.search = "<!**" + KEY_COLOUI_LOCATION + "**!>";
    sr.replace = colouisrc;
    strs << sr;

    sr.search = "<!**" + KEY_PROJECT_NAME + "**!>";
    sr.replace = pname;
    strs << sr;

    strs << updateQRC(pdir.absolutePath());

    // Creating pro file
    QString dest = pdir.absolutePath() + "/" + pname + ".pro";
    QString source = ":/assets/templates/profile.txt";

    if (!genFile(strs,source,dest)){
        showError("Could not open file " + dest + " for writing");
        return;
    }

    // Creating main file
    dest = pdir.absolutePath() + "/main.cpp";
    source = ":/assets/templates/main.txt";

    if (!genFile(strs,source,dest)){
        showError("Could not open file " + dest + " for writing");
        return;
    }

    // Creating the header file.
    dest = pdir.absolutePath() + "/" + mainw.toLower() + ".h";
    source = ":/assets/templates/window_h.txt";

    if (!genFile(strs,source,dest)){
        showError("Could not open file " + dest + " for writing");
        return;
    }

    // Creating the QRC file
    dest = pdir.absolutePath() + "/assets.qrc";
    source = ":/assets/templates/assets.txt";
    if (!genFile(strs,source,dest)){
        showError("Could not open file " + dest + " for writing");
        return;
    }

    // Creating the elements file
    elementsFile = pdir.absolutePath() + "/elements.h";
    QStringList if_else_chain;
    if (!generateElementsFile(elementsFile,&if_else_chain)){
        showError("Could not creates elements.h file");
        return;
    }

    // Creating main window class now that the if else chain can be created.

    QString chain = "";
    QString tab = "    ";

    if (if_else_chain.size() > 0){
        chain = "if ( info.elementID == " + if_else_chain.first()  + "){\n" + tab + "}\n";
        for (qint32 i = 1; i < if_else_chain.size(); i++){
            chain = chain + tab + "else if (info.elementID == " + if_else_chain.at(i) + " ){\n" + tab + "}\n";
        }
    }

    sr.search = "<!**" + KEY_IF_ELSE_CHAIN + "**!>";
    sr.replace = chain;
    strs << sr;

    dest = pdir.absolutePath() + "/" + mainw.toLower() + ".cpp";
    source = ":/assets/templates/window.txt";

    if (!genFile(strs,source,dest)){
        showError("Could not open file " + dest + " for writing");
        return;
    }

    this->done(0);

}

bool ProjectBuilder::generateElementsFile(QString filename, QStringList *if_else_chain){

    QFile file(filename);
    if (!file.open(QFile::WriteOnly)){
        showError("Could not creates elements.h file");
        return false;
    }

    // Creating the defines
    QStringList defines;
    qint32 maxL = 0;
    for (qint32 i = 0; i < uiElements.size(); i++){
        QString def = "#define   ";
        QString s = uiElements.at(i);
        s = s.replace(" ","_");
        s = s.replace(".","_");
        s = "E_" + s.toUpper();
        if (if_else_chain != nullptr){
            *if_else_chain << s;
        }
        def = def + s;
        if (maxL < def.length()){
            maxL = def.length();
        }
        defines << def;
    }

    QTextStream writer(&file);
    writer << "#ifndef ELEMENTS_H\n";
    writer << "#define ELEMENTS_H\n\n\n";

    for (qint32 i = 0; i < defines.size(); i++){

        QString def = defines.at(i);
        qint32 nspaces = maxL - def.size() + 3;
        QString spaces; spaces.fill(' ',nspaces);

        writer << def + spaces + "\"" + uiElements.at(i) + "\"\n";

    }

    writer << "\n\n#endif// ELEMENTS_H\n";
    file.close();

    return true;
}

bool ProjectBuilder::genFile(QVector<SearchAndReplace> strs, QString source, QString dest){
    QFile input(source);
    if (!input.open(QFile::ReadOnly)){
        qDebug() << "Failing on input" << source;
        return false;
    }

    QTextStream reader(&input);
    QString contents = reader.readAll();
    input.close();

    for (qint32 i = 0; i < strs.size(); i++){
        contents = contents.replace(strs.at(i).search,strs.at(i).replace);
    }

    QFile output(dest);
    if (!output.open(QFile::WriteOnly)){
        return false;
    }

    QTextStream writer(&output);
    writer << contents;
    output.close();

    return true;
}

void ProjectBuilder::on_pbUpdate_clicked()
{
    QString uifile = ui->leProjectLocation->text() + "/assets/ui_descriptor.cui";
    QString elementsh = ui->leProjectLocation->text() + "/elements.h";

    // Copying the ui file
    QFile duifile(uifile);
    if (duifile.exists()){
        duifile.remove();
    }

    if (!QFile::copy(uiFile,duifile.fileName())){
        showError("Could not copy ui file: " + uiFile + " to " + duifile.fileName());
        return;
    }

    if (!generateElementsFile(elementsh)){
        showError("Could not create elements file");
        return;
    }

    finalUiFile = uifile;
    elementsFile = elementsh;

    // Updating QRC and possibly new assets;

    QVector<SearchAndReplace> strs;
    strs << updateQRC(ui->leProjectLocation->text());

    QString dest = ui->leProjectLocation->text() + "/assets.qrc";
    QString source = ":/assets/templates/assets.txt";
    if (!genFile(strs,source,dest)){
        showError("Could not open file " + dest + " for writing");
        return;
    }

    this->done(0);
}

ProjectBuilder::SearchAndReplace ProjectBuilder::updateQRC(QString ploc){

    // Generating asset list and copying the assets files
    QString assetList = "";
    QString assetDir = ploc + "/assets";
    for (qint32 i = 0; i < assetsFiles.size(); i++){
        QFileInfo info(assetsFiles.at(i));
        assetList = assetList + "<file>assets/" + info.fileName() + "</file>\n";
        QString d = assetDir + "/" + info.fileName();

        // Removing the file if it exists to make sure to update.
        if (QFile(d).exists()){
            QFile(d).remove();
        }
        QFile::copy(assetsFiles.at(i),d);
    }

    // Copying the font files used by the keyboard
    QFile::copy(":/assets/unispace_reg.ttf",assetDir + "/unispace_reg.ttf");
    QFile::copy(":/assets/unispace_b.ttf",assetDir + "/unispace_b.ttf");

    SearchAndReplace sr;
    sr.replace = assetList;
    sr.search = "<!**" + KEY_QRC_ASSETS + "**!>";
    return sr;

}
