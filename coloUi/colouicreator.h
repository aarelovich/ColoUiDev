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

// Used for unifying file
const QString CHANGE_FILE_SEQUENCE = QString("!#$<<<<<");

class ColoUiCreator
{
public:
    ColoUiCreator();
    void createUi(QString file, ColoUiContainer *c);
    CreatorError getError() const {return error;}

private:

    ColoUiContainer *canvas;

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

    qint32 lineCounter;
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
    bool joinCuiFiles(QString masterFile, QString outputFile);

};

#endif // COLOUICREATOR_H
