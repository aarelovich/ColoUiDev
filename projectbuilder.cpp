#include "projectbuilder.h"
#include "ui_projectbuilder.h"

ProjectBuilder::ProjectBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectBuilder)
{
    ui->setupUi(this);
    showError("");
}

void ProjectBuilder::setupBuild(QStringList elements,
                                QString pname,
                                QString floc,
                                QString lastPLoc,
                                QString assets){
    uiElements = elements;
    ui->leProjectName->setText(pname);
    ui->leColoUiFolderLocation->setText(floc);
    ui->leProjectLocation->setText(lastPLoc);
    assetsSource = assets;

    // Hiding what doesn't go
    ui->pbUpdate->setVisible(false);
    ui->pbSearch->setEnabled(true);
    ui->pbBuild->setVisible(true);
    ui->leColoUiFolderLocation->setEnabled(true);
    ui->leMainWindow->setEnabled(true);
    ui->leProjectName->setEnabled(true);

}

void ProjectBuilder::setupUpdate(QString ploc, QStringList elements, QString assets){

    uiElements = elements;
    assetsSource = assets;

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

    QString where = QFileDialog::getExistingDirectory(this,"Qt Creator Project Location",ui->leProjectLocation->text());
    if (where.isEmpty()) return;
    ui->leProjectLocation->setText(where);

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
    QDir pdir(ploc + "/" + pname);

    // Cloning the assets directory
    QString assetsDestination = pdir.absolutePath() +"/assets";
    cloneAndUpdateDirectory(assetsSource, assetsDestination);
    // Copying the font files used by the keyboard
    QFile::copy(":/assets/unispace_reg.ttf",assetsDestination + "/unispace_reg.ttf");
    QFile::copy(":/assets/unispace_b.ttf",assetsDestination + "/unispace_b.ttf");


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

    strs << updateQRC(assetsDestination);

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
    QString elementsFile = pdir.absolutePath() + "/elements.h";
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

    QString assetsDestination = ui->leProjectLocation->text() +"/assets";

    // Checking for existance of CUI File
    if (!QFile(assetsDestination + "/ui_descriptor.cui").exists()){
        showError("Descriptor CUI file could not be found on selected location. Create the project first");
        return;
    }

    cloneAndUpdateDirectory(assetsSource, assetsDestination);
    // Copying the font files used by the keyboard
    QFile::copy(":/assets/unispace_reg.ttf",assetsDestination + "/unispace_reg.ttf");
    QFile::copy(":/assets/unispace_b.ttf",assetsDestination + "/unispace_b.ttf");

    QString elementsh = ui->leProjectLocation->text() + "/elements.h";
    if (!generateElementsFile(elementsh)){
        showError("Could not create elements file");
        return;
    }


    // Updating QRC and possibly new assets;
    QVector<SearchAndReplace> strs;
    strs << updateQRC(assetsSource);

    QString dest = ui->leProjectLocation->text() + "/assets.qrc";
    QString source = ":/assets/templates/assets.txt";
    if (!genFile(strs,source,dest)){
        showError("Could not open file " + dest + " for writing");
        return;
    }

    this->done(0);
}

ProjectBuilder::SearchAndReplace ProjectBuilder::updateQRC(QString assetsDir){

    QList<QString> assets = recursiveFileList(assetsDir);
    qint32 start = assetsDir.size();
    QString assetList = "";
    for (qint32 i = 0; i < assets.size(); i++){
        assetList = assetList + "        <file>assets" + assets.at(i).mid(start) + "</file>\n";
    }

    SearchAndReplace sr;
    sr.replace = assetList;
    sr.search = "<!**" + KEY_QRC_ASSETS + "**!>";
    return sr;

}

QStringList ProjectBuilder::recursiveFileList(QString rootDir){

    QStringList ans;
    QDir root(rootDir);
    if (!root.exists()) return ans;

    ans = root.entryList(QStringList(),QDir::Files);

    for (qint32 i = 0; i < ans.size(); i++){
        ans[i] = rootDir + "/" + ans.at(i);
    }

    QStringList dirList = root.entryList(QStringList(),QDir::Dirs|QDir::NoDotAndDotDot);

    // Calling this functions in all remaining directories
    for (qint32 i = 0; i < dirList.size(); i++){
        ans << recursiveFileList(rootDir+"/"+dirList.at(i));
    }

    return ans;

}

void ProjectBuilder::cloneAndUpdateDirectory(QString source, QString destination){

    //qDebug() << "S: " << source << "D: " << destination;

    QDir s(source);
    if (!s.exists()) return;

    QDir d(destination);
    if (!d.exists()){
        // Creating directory if parent exists.
        if (!d.cdUp()) return;
        if (!d.mkpath(destination)) return;
    }

    QStringList dirList = s.entryList(QStringList(),QDir::Dirs|QDir::NoDotAndDotDot);
    QStringList fileList = s.entryList(QStringList(),QDir::Files);

    // Copying all files.
    for (qint32 i = 0; i < fileList.size(); i++){
        QString dest = destination + "/" + fileList.at(i);
        if (QFile(dest).exists()){
            QFile(dest).remove();
        }
        QFile::copy(source + "/" + fileList.at(i),dest);
    }

    // Calling this functions in all remaining directories
    for (qint32 i = 0; i < dirList.size(); i++){
        cloneAndUpdateDirectory(source+"/"+dirList.at(i),destination+"/"+dirList.at(i));
    }

}
