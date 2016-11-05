#ifndef COLOUICREATOR_H
#define COLOUICREATOR_H

#include "colouicontainer.h"
#include <QDir>

struct CreatorError {
    QString error;
    qint32 line;
};

struct ColorResult{
    QVariantHash colorInfo;
    bool ok;
};

struct ConfigResult{
    ColoUiConfiguration config;
    QString endWord;
    bool ok;
};

struct UiDefinition{
    QString name;
    QString icon;
    qint32 line;
    QString file;
};


const QString PRJ_SOURCES_DIR = QString("src");
const QString PRJ_ASSESTS_DIR = QString("assets");
const QString PRJ_PROC_CUI_FILE = QString("ui_descriptor.cui");

class ColoUiCreator
{
public:

    ColoUiCreator();
    void createUi(QString masterFile,
                  QString projectDirectory,
                  ColoUiContainer *c);
    void createFromResource(ColoUiContainer *c);
    CreatorError getError() const {return error;}
    QVector<UiDefinition> getDefinitions() const {return uiDefinitions;}
    QStringList getGlobalConfigs() const {return globalConfigs.keys();}

private:

    const QString ICON_COLOR    = QString(":/assets/color.png");
    const QString ICON_GRADIENT = QString(":/assets/gradient.png");
    const QString ICON_CONFIGS  = QString(":/assets/configs.png");
    const QString ICON_FONT     = QString(":/assets/font.png");

    // Used for unifying file
    const QString CHANGE_FILE_START_SEQUENCE = QString("!#$<<<<<");
    const QString CHANGE_FILE_END_SEQUENCE = QString("!#$>>>>>");

    // Container where the views will be constructed.
    ColoUiContainer *canvas;

    // Used to generate list.
    QVector<UiDefinition> uiDefinitions;

    // Properties by type
    QStringList gradientAcceptProperties;
    QStringList colorProperties;
    QStringList oneUintProperties;
    QStringList oneIntProperties;
    QStringList oneStringProperties;
    QStringList onePositionProperties;
    QStringList oneShapeProperties;
    QStringList oneBoolProperties;

    // Configs and colors
    QHash<QString,ColoUiConfiguration> globalConfigs;
    QHash<QString,QVariantHash> globalGradientsAndColors;

    QVector<qint32> lineCounter;
    QStringList filesBeingParsed;
    CreatorError error;

    bool drawAreaEstablished;
    QString assetsFolder;

    QStringList getNextLineOfCode(QTextStream *stream);
    QStringList tokenizeLine(QString line);


    //------------- Parsing functions ------------
    ColorResult parseColorInfo(QStringList tokens);
    bool parseDrawArea(QTextStream *stream);
    ConfigResult parseConfig(QTextStream *stream,
                             bool enableUseConfig,
                             QStringList endDeclarations,
                             QStringList mandatory,
                             QString ReservedWord);
    bool parseView(QTextStream *stream);
    bool parseList(QTextStream *stream, ColoUiView *view);
    bool parseDropdown(QTextStream *stream, ColoUiView *view);
    bool parseConfigLike(QString langWord, ColoUiView *view, QTextStream *stream);

    //----------------- Document join ----------------
    bool joinCuiFiles(QString masterFile, QString outputFile, QString workingDir);

};

#endif // COLOUICREATOR_H
