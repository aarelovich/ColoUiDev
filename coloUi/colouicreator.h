#ifndef COLOUICREATOR_H
#define COLOUICREATOR_H

#include "colouicontainer.h"

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


class ColoUiCreator
{
public:
    ColoUiCreator();
    void createUi(QString masterFile, QString globalFile, QString workingDir, ColoUiContainer *c);
    CreatorError getError() const {return error;}
    QVector<UiDefinition> getDefinitions() const {return uiDefinitions;}

private:

    const QString ICON_COLOR    = QString(":/assets/color.png");
    const QString ICON_GRADIENT = QString(":/assets/gradient.png");
    const QString ICON_CONFIGS  = QString(":/assets/configs.png");

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

    QHash<QString,ColoUiConfiguration> globalConfigs;
    QHash<QString,QVariantHash> globalGradientsAndColors;

    QVector<qint32> lineCounter;
    QStringList filesBeingParsed;
    CreatorError error;

    bool drawAreaEstablished;

    QStringList getNextLineOfCode(QTextStream *stream);
    QStringList tokenizeLine(QString line);    
    ColoUiConfiguration completeBasicItemConfiguration(ColoUiConfiguration c);


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

    //----------------- Document join ----------------
    bool joinCuiFiles(QString masterFile, QString outputFile, QString workingDir);

};

#endif // COLOUICREATOR_H
