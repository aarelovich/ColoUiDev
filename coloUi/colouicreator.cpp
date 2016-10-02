#include "colouicreator.h"

ColoUiCreator::ColoUiCreator()
{
    gradientAcceptProperties << CPR_BACKGROUND_COLOR
                             << CPR_ALTERNATIVE_BACKGROUND_COLOR
                             << CPR_ALTERNATIVE_TEXT_COLOR
                             << CPR_TEXT_COLOR;
    colorProperties = gradientAcceptProperties;
    colorProperties << CPR_BORDER_COLOR;

    oneBoolProperties << CPR_VALUES_RELATIVE << CPR_READ_ONLY << CPR_ALTERNATIVE_BACKGROUND_ON_HOVER <<  CPR_LIST_HEADER_VISIBLE << CPR_USE_HTML;

    onePositionProperties << CPR_TRANSITION_TYPE << CPR_ICON_POSITION;

    oneShapeProperties << CPR_SHAPE;

    oneStringProperties << CPR_NAME << CPR_ICON_PATH << CPR_TEXT << CPR_TRANSITION_VIEW_A << CPR_TRANSITION_VIEW_B;

    oneUintProperties << CPR_BORDER_WIDTH << CPR_HEIGHT << CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST
                      << CPR_ROUNDED_RECT_RADIOUS << CPR_TRANSITION_STEPS
                      << CPR_TRANSITION_TIME << CPR_WIDTH << CPR_X << CPR_Y;

    oneIntProperties << CPR_X_OFFSET << CPR_Y_OFFSET;

}

void ColoUiCreator::createFromResource(ColoUiContainer *c){
    QString res = ":/assets/ui_descriptor.cui";
    createUi("",res,"",c,true);
}

void ColoUiCreator::createUi(QString masterFile, QString globalFile, QString workingDir , ColoUiContainer *c, bool noJoin){


    if (!noJoin){
        // Creating a single master file
        if (!joinCuiFiles(masterFile,globalFile,workingDir)){
            return;
        }
    }

    QFile f(globalFile);
    if (!f.open(QFile::ReadOnly)){
        error.error = "Could not open " + f.fileName() + " for reading";
        error.line = 0;
        return;
    }

    canvas = c;
    canvas->deleteUi();

    globalConfigs.clear();
    globalGradientsAndColors.clear();

    lineCounter.clear();
    filesBeingParsed.clear();

    lineCounter << 0;
    filesBeingParsed << masterFile;

    drawAreaEstablished = false;

    //qDebug() << "About to parse";
    uiDefinitions.clear();

    QTextStream reader(&f);
    while (!reader.atEnd()){

        QString line = reader.readLine();
        lineCounter[lineCounter.size()-1]++;

        // Before tokenization, the join annotations need to be checked.
        if (line.startsWith(CHANGE_FILE_START_SEQUENCE)){
            QString fname = line.remove(CHANGE_FILE_START_SEQUENCE);
            fname = fname.trimmed();
            filesBeingParsed << fname;
            lineCounter << 0;
            continue;
        }
        else if (line.startsWith(CHANGE_FILE_END_SEQUENCE)){
            filesBeingParsed.removeLast();
            lineCounter.removeLast();
            continue;
        }

        QStringList tokens = tokenizeLine(line);

        if (tokens.isEmpty()) continue;

        QString rword = tokens.first();
        tokens.removeFirst();

        if (rword == CUI_LANG_COLOR){

            if (tokens.size() != 2){
                error.error = "On file "  + filesBeingParsed.last() + ":  Color declaration expects exactly two parameters and " + QString::number(tokens.size()) + " were found";
                error.line = lineCounter.last();
                f.close();
                return;
            }

            QString name = tokens.first();
            if (name.startsWith('#')){
                error.error = "On file "  + filesBeingParsed.last() + ":  Invalid color name " + name +  " in declaration. Name cannot start with #";
                error.line = lineCounter.last();
                f.close();
                return;
            }

            if (!QColor::isValidColor(tokens.last())){
                error.error = "On file "  + filesBeingParsed.last() + ":  Invalid color defined in " + name + ": " + tokens.last();
                error.line = lineCounter.last();
                f.close();
                return;
            }

            if (globalGradientsAndColors.contains(name)){
                error.error = "On file "  + filesBeingParsed.last() + ":  Invalid color name in " + name + " as it was used in a previous gradient or color";
                error.line = lineCounter.last();
                f.close();
                return;
            }

            QStringList colors;
            colors << tokens.last();
            QVariantHash m;
            m[INTERNAL_COLOR_LIST] = colors;
            m[INTERNAL_GRAD_TYPE] = CPA_GRAD_NONE;

            globalGradientsAndColors[name] = m;

            // Adding the definitions
            UiDefinition def;
            def.name = name;
            def.icon = ICON_COLOR;
            def.line = lineCounter.last();
            def.file = filesBeingParsed.last();
            uiDefinitions << def;

        }
        else if (rword ==  CUI_LANG_CONFIG){        
            if (tokens.isEmpty()){

                ConfigResult res = parseConfig(&reader,false,QStringList(),QStringList(),"");
                if (!res.ok){
                    f.close();
                    return;
                }

                QString name = res.config.getString(CPR_NAME);
                res.config.removeProperty(CPR_NAME);
                if (name.isEmpty()){
                    error.error = "On file "  + filesBeingParsed.last() + ":  Name for global configuration was not set.";
                    error.line = lineCounter.last();
                    f.close();
                    return;
                }

                globalConfigs[name] = res.config;

                // Adding to the definitions
                UiDefinition def;
                def.name = name;
                def.icon = ICON_CONFIGS;
                def.line = lineCounter.last();
                def.file = filesBeingParsed.last();
                uiDefinitions << def;


            }
            else{
                error.error = rword + " should contain no other parameters";
                error.line = lineCounter.last();
                f.close();
                return;
            }
        }
        else if (rword == CUI_LANG_DRAW_AREA){
            if (tokens.isEmpty()){
                if (!parseDrawArea(&reader)){
                    f.close();
                    return;
                }
            }
            else{
                error.error = rword + " should contain no other parameters";
                error.line = lineCounter.last();
                f.close();
                return;
            }
        }
        else if (rword == CUI_LANG_GRADIENT){            
            if (!tokens.isEmpty()){

                QString name = tokens.first();
                tokens[0] = CPR_BACKGROUND_COLOR;
                ColorResult co = parseColorInfo(tokens);
                if (!co.ok){
                    f.close();
                    return;
                }
                if (co.colorInfo.value(INTERNAL_GRAD_TYPE).toUInt() == CPA_GRAD_NONE){
                    error.error = "On file "  + filesBeingParsed.last() + ":  On Global Gradient declaration, found color declaration";
                    error.line = lineCounter.last();
                    f.close();
                    return;
                }

                if (globalGradientsAndColors.contains(name)){
                    error.error = "On file "  + filesBeingParsed.last() + ":  Invalid color name in " + name + " as it was used in a previous gradient or color";
                    error.line = lineCounter.last();
                    f.close();
                    return;
                }

                globalGradientsAndColors[name] = co.colorInfo;

                // Adding to the definitions
                UiDefinition def;
                def.name = name;
                def.icon = ICON_GRADIENT;
                def.line = lineCounter.last();
                def.file = filesBeingParsed.last();
                uiDefinitions << def;

            }
            else{
                error.error = rword + " should contain exactly 4 parameters";
                error.line = lineCounter.last();
                f.close();
                return;
            }

        }
        else if (rword == CUI_LANG_TRANSITION){

            QStringList mandatoryForTransition;
            mandatoryForTransition << CPR_TRANSITION_VIEW_A << CPR_TRANSITION_VIEW_B
                                   << CPR_TRANSITION_STEPS << CPR_TRANSITION_TIME
                                   << CPR_TRANSITION_TYPE;

            ConfigResult res = parseConfig(&reader,true,QStringList(),mandatoryForTransition,rword);
            if (!res.ok){
                f.close();
                return;
            }

            QString e = canvas->addTransition(res.config);
            if (!e.isEmpty()){
                error.error = "On file "  + filesBeingParsed.last() + ":  Error while creating transition: " + e;
                error.line = lineCounter.last();
                f.close();
                return;
            }

        }
        else if (rword == CUI_LANG_VIEW){

            if (!parseView(&reader)){
                f.close();
                return;
            }

        }
        else{
            error.error = "On file "  + filesBeingParsed.last() + ":  Unexpected global declaration " + rword;
            error.line = lineCounter.last();
            f.close();
            return;
        }

    }

    //qDebug() << "About to draw";

    f.close();
    canvas->drawUi();

}

QStringList ColoUiCreator::getNextLineOfCode(QTextStream *stream){
    while (!stream->atEnd()){
        QString line = stream->readLine();
        QStringList tokens = tokenizeLine(line);
        //qDebug() << "Line" << line << "TOKENIZED" << tokens;
        lineCounter[lineCounter.size()-1]++;
        if (!tokens.isEmpty()){
            return tokens;
        }
    }
    // If it got here the stream has ended.
    return QStringList();
}

QStringList ColoUiCreator::tokenizeLine(QString line){

    // Getting rid of the comments.

    qint32 pI = line.indexOf("%");
    line = line.left(pI);
    //qDebug() << "Line with no coments";

    // Dodging empty lines.
    if (line.isEmpty()) return QStringList();

    //qDebug() << "Line before paramters" << line;

    QStringList tokens = line.split("|");

    if (tokens.isEmpty()) return QStringList();

    // Removing extra white space
    QStringList resultingTokens;
    for (qint32 i = 0; i < tokens.size(); i++){
        QString temp = tokens.at(i).trimmed();
        if (!temp.isEmpty()){
            resultingTokens << temp;
        }
    }

    //qDebug() << "Resulting tokens" << tokens;

    return resultingTokens;
    //return tokens;
}

ColorResult ColoUiCreator::parseColorInfo(QStringList tokens){

    ColorResult ret;
    ret.ok = false;

    if (tokens.isEmpty()){
        error.error = "On file "  + filesBeingParsed.last() + ":  Color option information is empty";
        return ret;
    }

    if (tokens.size() > 2){
        // It is a gradient
        if (!gradientAcceptProperties.contains(tokens.first())){
            error.error = "On file "  + filesBeingParsed.last() + ":  Property " + tokens.first() + " does not accept color gradient";
            error.line = lineCounter.last();
            return ret;
        }

        if (tokens.size() != 4){
            error.error = "On file "  + filesBeingParsed.last() + ":  Gradient definition expects exactly 4 parameters. Found " + QString::number(tokens.size()-1);
            error.line = lineCounter.last();
            return ret;
        }

        QString gradType = tokens.at(1);
        QString color1   = tokens.at(2);
        QString color2   = tokens.at(3);

        if (!ColoUiParameters.contains(gradType)){
            error.error = "On file "  + filesBeingParsed.last() + ":  Unrecognized gradient type " + gradType;
            error.line = lineCounter.last();
            return ret;
        }
        quint16 gradtype = ColoUiParameters.value(gradType);

        if (!ColoUiGradType.contains(gradtype)){
            error.error = "On file "  + filesBeingParsed.last() + ":  Invalid gradient type " + gradType;
            error.line = lineCounter.last();
            return ret;
        }

        ret.colorInfo[INTERNAL_GRAD_TYPE] = gradtype;
        if (!QColor::isValidColor(color1)){
            error.error = "On file "  + filesBeingParsed.last() + ":  First color of gradient " + color1 + " is an invalid color";
            error.line = lineCounter.last();
            return ret;
        }
        if (!QColor::isValidColor(color2)){
            error.error = "On file "  + filesBeingParsed.last() + ":  Second color of gradient " + color2 + " is an invalid color";
            error.line = lineCounter.last();
            return ret;
        }

        QStringList colors;
        colors << color1 << color2;

        ret.colorInfo[INTERNAL_COLOR_LIST] = colors;
    }
    else{
        // It is either a color or a name.
        QString p = tokens.last();
        if (p.startsWith('#')){
            // It is a color
            if (!QColor::isValidColor(p)){
                error.error = "On file "  + filesBeingParsed.last() + ":  Color string " + p + " is invalid";
                error.line = lineCounter.last();
                return ret;
            }
            ret.colorInfo[INTERNAL_GRAD_TYPE] = CPA_GRAD_NONE;
            QStringList colors;
            colors << p;
            ret.colorInfo[INTERNAL_COLOR_LIST] = colors;
        }
        else{
            // It is a name
            if (globalGradientsAndColors.contains(p)){
                ret.colorInfo = globalGradientsAndColors.value(p);

                if (!gradientAcceptProperties.contains(tokens.first())){
                    if (ret.colorInfo.value(INTERNAL_GRAD_TYPE).toUInt() != CPA_GRAD_NONE){
                        error.error = "On file "  + filesBeingParsed.last() + ":  Property " + tokens.first() + " does not accept color gradient";
                        error.line = lineCounter.last();
                        return ret;
                    }
                }

            }
            else{
                error.error = "On file "  + filesBeingParsed.last() + ":  Parameter " + p + " is a invalid color or color/gradient name";
                error.line = lineCounter.last();
                return ret;
            }
        }
    }

    ret.ok = true;
    return ret;

}

bool ColoUiCreator::parseDrawArea(QTextStream *stream){

    bool done = false;

    quint16 width = 0;
    quint16 height = 0;
    QColor background(Qt::white);

    while (!done){

        QStringList list = getNextLineOfCode(stream);
        if (list.isEmpty()){
            error.error = "On file "  + filesBeingParsed.last() + ":  End of document found without finding the DONE for DRAW_AREA";
            error.line = lineCounter.last();
            return false;
        }

        if (list.first() == CPR_WIDTH){
            if (list.size() != 2){
                error.error = "On file "  + filesBeingParsed.last() + ":  width property should only have one parameter";
                error.line = lineCounter.last();
                return false;
            }
            QString num = list.last();
            bool ok = false;
            width = num.toUInt(&ok);
            if (!ok){
                error.error = "On file "  + filesBeingParsed.last() + ":  The value of width " + num + " is not a valid positive interger";
                error.line = lineCounter.last();
                return false;
            }
        }
        else if (list.first() == CPR_HEIGHT){
            if (list.size() != 2){
                error.error = "On file "  + filesBeingParsed.last() + ":  height property should only have one parameter";
                error.line = lineCounter.last();
                return false;
            }
            QString num = list.last();
            bool ok = false;
            height = num.toUInt(&ok);
            if (!ok){
                error.error = "On file "  + filesBeingParsed.last() + ":  The value of height " + num + " is not a valid positive interger";
                error.line = lineCounter.last();
                return false;
            }
        }
        else if (list.first() == CPR_BACKGROUND_COLOR){
            ColorResult co = parseColorInfo(list);
            if (!co.ok){
                return false;
            }
            if (co.colorInfo.value(INTERNAL_GRAD_TYPE).toUInt() != CPA_GRAD_NONE){
                error.error = "On file "  + filesBeingParsed.last() + ":  DRAW_AREA Cannot use a gradient as its background color";
                error.line = lineCounter.last();
                return false;
            }
            background = QColor(co.colorInfo.value(INTERNAL_COLOR_LIST).toStringList().first());
        }
        else if (list.first() == CUI_LANG_DONE){
            done = true;
        }
        else{
            error.error = "On file "  + filesBeingParsed.last() + ":  Unexpected DRAW_AREA property " + list.first();
            error.line = lineCounter.last();
            return false;
        }
    }

    if (width == 0 || height == 0){
        error.error = "On file "  + filesBeingParsed.last() + ":  DRAW_AREA should have non zero width and non zero height";
        error.line = lineCounter.last();
        return false;
    }

    canvas->setDrawingArea(width,height);
    canvas->setTransitionScreenColor(background);

    return true;

}

ConfigResult ColoUiCreator::parseConfig(QTextStream *stream,
                                        bool enableUseConfig,
                                        QStringList endDeclarations,
                                        QStringList mandatory,
                                        QString ReservedWord){

    ConfigResult res;
    res.ok = false;

    if (!endDeclarations.contains(CUI_LANG_DONE)){
        endDeclarations << CUI_LANG_DONE;
    }

    bool done = false;
    while (!done){

        QStringList list = getNextLineOfCode(stream);
        //qDebug() << "Parse config list " << list;
        if (list.isEmpty()){
            error.error = "On file "  + filesBeingParsed.last() + ":  End of document found without finding the any set end declaration for configuration";
            error.line = lineCounter.last();
            return res;
        }

        //qDebug() << "Tokens in list for parseConfig: " << list;

        bool dontOneProcess = false;

        if (colorProperties.contains(list.first())){
            ColorResult co = parseColorInfo(list);
            if (!co.ok){
                return res;
            }
            res.config.set(list.first(),co.colorInfo);
            dontOneProcess = true;
        }
        else if (list.first() == CPR_FONT){

            if (list.size() >= 3){
                QString family = list.at(1);
                bool ok;
                quint16 ps = list.at(2).toUInt(&ok);
                if (!ok){
                    error.error = "On file "  + filesBeingParsed.last() + ":  Font property must have the second parameter (Size) be a valid postive integer. Found " + list.at(2);
                    error.line = lineCounter.last();
                    return res;
                }

                QVariantHash font;
                font[INTERNAL_FONT_FAMILY] = family;
                font[INTERNAL_FONT_SIZE] = ps;
                font[INTERNAL_FONT_ITALIC] = false;
                font[INTERNAL_FONT_BOLD] = false;

                for (qint32 i = 3; i < list.size(); i++){

                    QString mod = list.at(i);

                    // Either Bold or Italic
                    if (ColoUiParameters.contains(mod)){
                        quint16 value = ColoUiParameters.value(mod);
                        if (ColoUiFontMods.contains(value)){
                            if (value == CPA_ITALIC){
                                font[INTERNAL_FONT_ITALIC] = true;
                            }
                            else if (value == CPA_BOLD){
                                font[INTERNAL_FONT_BOLD] = true;
                            }
                        }
                        else{
                            error.error = "On file "  + filesBeingParsed.last() + ":  Invalid font property found " + mod + ". Expecting Italic or Bold";
                            error.line = lineCounter.last();
                            return res;
                        }
                    }
                    else{
                        error.error = "On file "  + filesBeingParsed.last() + ":  Invalid font property found " + mod + ". Expecting Italic or Bold";
                        error.line = lineCounter.last();
                        return res;
                    }
                }

                res.config.set(CPR_FONT,font);
            }
            else{
                error.error = "On file "  + filesBeingParsed.last() + ":  Font property must have at least two parameters";
                error.line = lineCounter.last();
                return res;
            }

            dontOneProcess = true;

        }
        else if (endDeclarations.contains(list.first())){
            done = true;
            res.endWord = list.first();
            dontOneProcess = true;
        }

        if (dontOneProcess) continue;


        if (list.size() != 2){
            error.error = "On file "  + filesBeingParsed.last() + ":  Property " + list.first() + " uses exactly one parameter";
            error.line = lineCounter.last();
            return res;
        }

        //qDebug() << "Reading property" << list.first();

        if (oneUintProperties.contains(list.first())){
            bool ok = false;
            quint16 val = list.last().toUInt(&ok);
            if (!ok){
                error.error = "On file "  + filesBeingParsed.last() + ":  Property " + list.first() + " requires positive integer as paramter. Found " + list.last();
                error.line = lineCounter.last();
                return res;
            }
            res.config.set(list.first(),val);
        }
        else if (oneIntProperties.contains(list.first())){
            bool ok = false;
            qint32 val = list.last().toInt(&ok);
            if (!ok){
                error.error = "On file "  + filesBeingParsed.last() + ":  Property " + list.first() + " requires an integer paramter. Found " + list.last();
                error.line = lineCounter.last();
                return res;
            }
            res.config.set(list.first(),val);
        }
        else if (oneStringProperties.contains(list.first())){
            res.config.set(list.first(),list.last());
        }
        else if (oneBoolProperties.contains(list.first())){
            bool b;
            if (list.last() == "true"){
                b = true;
            }
            else if (list.last() == "false"){
                b = false;
            }
            else{
                error.error = "On file "  + filesBeingParsed.last() + ":  Property " + list.first() + " should have one boolean parameter (true or false)";
                error.line = lineCounter.last();
                return res;
            }
            //qDebug() << "Setting property " << list.first() << "to" << b;
            res.config.set(list.first(),b);
        }
        else if (onePositionProperties.contains(list.first())){

            qint32 index = -1;
            if (ColoUiParameters.contains(list.last())){
                index = ColoUiParameters.value(list.last());
                if (!ColoUiDirection.contains(index)){
                    index = -1;
                }
            }

            if (index == -1){
                error.error = "On file "  + filesBeingParsed.last() + ":  Property " + list.first() + " requires a direction parameter: Up,Down,Right,Left. Found " + list.last();
                error.line = lineCounter.last();
                return res;
            }

            res.config.set(list.first(),index);

        }
        else if (oneShapeProperties.contains(list.first())){

            qint32 index = -1;
            if (ColoUiParameters.contains(list.last())){
                index = ColoUiParameters.value(list.last());
                if (!ColoUiShape.contains(index)){
                    index = -1;
                }
            }

            if (index == -1){
                error.error = "On file "  + filesBeingParsed.last() + ":  Property " + list.first() + " requires a shape parameter: Rect, Ellipse, RoundRect. Found " + list.last();
                error.line = lineCounter.last();
                return res;
            }

            res.config.set(list.first(),index);

        }
        else if (list.first() == CPR_USE_CONFIGURATION){
            if (!enableUseConfig){
                error.error = CPR_USE_CONFIGURATION;
                error.error = error.error + " cannot be used in this type of declaration";
                error.line = lineCounter.last();
                return res;
            }

            if (globalConfigs.contains(list.last())){
                res.config = globalConfigs.value(list.last());
            }
            else{
                error.error = "On file "  + filesBeingParsed.last() + ":  Request for non-existent configuration " + list.last();
                error.line = lineCounter.last();
                return res;
            }

            dontOneProcess = true;

        }
        else{
            error.error = "On file "  + filesBeingParsed.last() + ":  Unexpected CONFIG property " + list.first();
            error.line = lineCounter.last();
            return res;
        }

    }

    // Checking all mandatory words are in here
    for (qint32 i = 0; i < mandatory.size(); i++){
        if (!res.config.has(mandatory.at(i))){
            error.error = "On file "  + filesBeingParsed.last() + ":  Required configuration for "  + ReservedWord +  " is incomplete before " + res.endWord + ". " + mandatory.at(i) + " missing";
            error.line = lineCounter.last();
            return res;
        }
    }

    res.ok = true;
    return res;
}


bool ColoUiCreator::parseView(QTextStream *stream){

    QStringList viewEnders;
    viewEnders << CUI_LANG_BUTTON << CUI_LANG_LIST << CUI_LANG_TEXT << CUI_LANG_DONE;

    QStringList mandatory;
    mandatory << CPR_X << CPR_Y << CPR_WIDTH << CPR_HEIGHT << CPR_NAME;

    ConfigResult cr = parseConfig(stream,false,viewEnders,mandatory,CUI_LANG_VIEW);

    if (!cr.ok){
        return false;
    }

    quint16 x;
    quint16 y;
    quint16 width;
    quint16 height;
    QString ID;
    bool dimAreRelative = false;

    if (cr.config.has(CPR_VALUES_RELATIVE)){
        dimAreRelative = cr.config.getBool(CPR_VALUES_RELATIVE);
    }
    else dimAreRelative = false;

    ID = cr.config.getString(CPR_NAME);
    x = cr.config.getUInt16(CPR_X);
    y = cr.config.getUInt16(CPR_Y);
    width = cr.config.getUInt16(CPR_WIDTH);
    height = cr.config.getUInt16(CPR_HEIGHT);

    QString e = canvas->createView(ID,x,y,width,height,dimAreRelative);
    if (!e.isEmpty()){
        error.error = "On file "  + filesBeingParsed.last() + ":  Error while creating view " + ID + ": " +e;
        error.line = lineCounter.last();
        return false;
    }

    if (cr.endWord == CUI_LANG_DONE) return true; // This means tha the view was declared without any element

    ColoUiView *view = canvas->getViewByID(ID);

    if (cr.config.has(CPR_BACKGROUND_COLOR)){
        view->setViewBackgroundColor(cr.config.getGradient(CPR_BACKGROUND_COLOR));
    }


    bool done = false;
    QStringList list;
    list << cr.endWord;

    while (!done){

        // Analyzing the elements
        if (list.first() == CUI_LANG_BUTTON){

            ConfigResult res = parseConfig(stream,true,QStringList(),mandatory,list.first());

            if (!res.ok){
                return false;
            }

            // Completing the configuration
            res.config = completeBasicItemConfiguration(res.config);

            QString e = view->createElement(CUI_BUTTON,
                                            res.config.getString(CPR_NAME),
                                            res.config,canvas->getSignalManager(),
                                            res.config.getBool(CPR_VALUES_RELATIVE));

            if (!e.isEmpty()){
                error.error = "On file "  + filesBeingParsed.last() + ":  Error creating button: " + res.config.getString(CPR_NAME) + " in view "
                        + view->getElementID() + ": " + e;
                error.line = lineCounter.last();
                return false;
            }


        }
        else if (list.first() == CUI_LANG_LIST){

            if (!parseList(stream,view)){
                return false;
            }

        }
        else if (list.first() == CUI_LANG_TEXT){

            ConfigResult res = parseConfig(stream,true,QStringList(),mandatory,list.first());

            if (!res.ok){
                return false;
            }

            // Completing the configuration
            res.config = completeBasicItemConfiguration(res.config);

            QString e = view->createElement(CUI_TEXT,
                                            res.config.getString(CPR_NAME),
                                            res.config,canvas->getSignalManager(),
                                            res.config.getBool(CPR_VALUES_RELATIVE));

            if (!e.isEmpty()){
                error.error = "On file "  + filesBeingParsed.last() + ":  Error creating text: " + res.config.getString(CPR_NAME) + " in view "
                        + view->getElementID() + ": " + e;
                error.line = lineCounter.last();
                return false;
            }


        }
        else{
            error.error = "On file "  + filesBeingParsed.last() + ":  Unexpected element declaration found in VIEW: " + list.first();
            error.line = lineCounter.last();
            return false;
        }

        list = getNextLineOfCode(stream);

        if (list.isEmpty()){
            error.error = "On file "  + filesBeingParsed.last() + ":  End of document found without finding the DONE for VIEW";
            error.line = lineCounter.last();
            return false;
        }

        if (list.first() == CUI_LANG_DONE){
            done = true;
        }

    }

    return true;

}


bool ColoUiCreator::parseList(QTextStream *stream, ColoUiView *view){


    QStringList viewEnders;
    viewEnders << CUI_LANG_HEADER << CUI_LANG_ITEM << CUI_LANG_DONE;

    QStringList mandatory;
    mandatory << CPR_X << CPR_Y << CPR_WIDTH << CPR_HEIGHT << CPR_NAME << CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST;

    ConfigResult cr = parseConfig(stream,true,viewEnders,mandatory,CUI_LANG_VIEW);

    //qDebug() << "On list after parsing first part, show headers is" << CPR_LIST_HEADER_VISIBLE << cr.config.getBool(CPR_LIST_HEADER_VISIBLE);

    if (!cr.ok){
        return false;
    }

    if (cr.endWord == CUI_LANG_DONE) return true;

    bool done = false;
    QStringList list;
    list << cr.endWord;

    QVector<ColoUiConfiguration> headers;
    QVector< QVector<ColoUiConfiguration> > items;


    while (!done){

        if (list.first() == CUI_LANG_HEADER){

            //QStringList headerMandatory = QStringList() << CPR_WIDTH << CPR_TEXT;
            ConfigResult res = parseConfig(stream,true,QStringList(),QStringList(),list.first());
            if (!res.ok){
                return false;
            }

            res.config = completeBasicItemConfiguration(res.config);
            headers << res.config;

        }
        else if (list.first() == CUI_LANG_ITEM){

            QStringList itemMandatory = QStringList() << CPR_X << CPR_Y; // X and Y represent the row and column
            ConfigResult res = parseConfig(stream,true,QStringList(),itemMandatory,list.first());
            if (!res.ok){
                return false;
            }

            quint16 row = res.config.getUInt16(CPR_X);
            quint16 col = res.config.getUInt16(CPR_Y);

            if (col >= headers.size()){
                error.error = "On file "  + filesBeingParsed.last() + ":  Declared column for item " + QString::number(col) + " exceeds number of defined header: "
                        + QString::number(headers.size());
                error.line = lineCounter.last();
                return false;
            }

            if (row < items.size()){
                items[row][col] = completeBasicItemConfiguration(res.config);
            }
            else if (row == items.size()){ // Add a new row

                QVector<ColoUiConfiguration> row;
                for (qint32 i = 0; i < headers.size(); i++){
                    row << ColoUiConfiguration();
                }
                row[col] = completeBasicItemConfiguration(res.config);
                items << row;

            }
            else{  // Cannot add more than one row at the time.
                error.error = "On file "  + filesBeingParsed.last() + ":  Declared row in item " + QString::number(row) + " is larger than the maximum allowed row "
                        + QString::number(items.size());
                error.line = lineCounter.last();
                return false;
            }

        }
        else{
            error.error = "On file "  + filesBeingParsed.last() + ":  Unexpected element declaration found in LIST: " + list.first();
            error.line = lineCounter.last();
            return false;
        }

        list = getNextLineOfCode(stream);

        if (list.isEmpty()){
            error.error = "On file "  + filesBeingParsed.last() + ":  End of document found without finding the DONE for LIST";
            error.line = lineCounter.last();
            return false;
        }

        if (list.first() == CUI_LANG_DONE){
            done = true;
        }

    }


    QString e = view->createElement(CUI_LIST,
                                    cr.config.getString(CPR_NAME),
                                    cr.config,canvas->getSignalManager(),
                                    cr.config.getBool(CPR_VALUES_RELATIVE));

    if (!e.isEmpty()){
        error.error = "On file "  + filesBeingParsed.last() + ":  Error while creating LIST on VIEW " + view->getElementID() + ": " + e;
        error.line = lineCounter.last();
        return false;
    }

    ColoUiList *coloList = (ColoUiList *)view->element(cr.config.getString(CPR_NAME));

    coloList->setConfiguration(cr.config);

    // Setting the header configuration
    for (qint32 i = 0; i < headers.size(); i++){
        if (!coloList->setHeaderConfig(headers.at(i))){ // Adding all headers
            error.error = "On file "  + filesBeingParsed.last() + ":  Attempting to set value of invalid header column in list creation" + QString::number(i);
            error.line = lineCounter.last();
            return false;
        }
    }

    // Adding all necessary rows
    for (qint32 i = 0; i < items.size(); i++){
        coloList->insertRow();
    }

    // Setting the item configurations
    for (qint32 i = 0; i < items.size(); i++){
        for (qint32 j = 0; j < items.at(i).size(); j++){
            if (!coloList->setItemConfiguration(i,j,items.at(i).at(j))){
                error.error = "On file "  + filesBeingParsed.last() + ":  Attempting to add item to invalid position " + QString::number(i) + ", " + QString::number(j) + " on list creation";
                error.line = lineCounter.last();
                return false;
            }
        }
    }

    return true;

}

ColoUiConfiguration ColoUiCreator::completeBasicItemConfiguration(ColoUiConfiguration c){
    if (!c.has(CPR_BACKGROUND_COLOR)){
        QStringList colors = QStringList() << "#444444";
        c.setGradient(CPR_BACKGROUND_COLOR,colors,CPA_GRAD_NONE);
    }

    if (!c.has(CPR_ALTERNATIVE_BACKGROUND_COLOR)){
        c.set(CPR_ALTERNATIVE_BACKGROUND_COLOR,ColoUiConfiguration::lightenColors(c.getGradient(CPR_BACKGROUND_COLOR)));
    }

    if (!c.has(CPR_TEXT_COLOR)){
        QStringList colors = QStringList() << "#000000";
        c.setGradient(CPR_TEXT_COLOR,colors,CPA_GRAD_NONE);
    }

    if (!c.has(CPR_ALTERNATIVE_TEXT_COLOR)){
        c.set(CPR_ALTERNATIVE_TEXT_COLOR,ColoUiConfiguration::lightenColors(c.getGradient(CPR_TEXT_COLOR)));
    }

    if (!c.has(CPR_BORDER_WIDTH)){
        c.set(CPR_BORDER_WIDTH,0);
    }

    if (!c.has(CPR_BORDER_COLOR)){
        QStringList colors = QStringList() << "#000000";
        c.setGradient(CPR_BORDER_COLOR,colors,CPA_GRAD_NONE);
    }

    if (!c.has(CPR_VALUES_RELATIVE)){
        c.set(CPR_VALUES_RELATIVE,false);
    }

    if (!c.has(CPR_USE_HTML)){
        c.set(CPR_USE_HTML,false);
    }

    return c;
}


//--------------------------------------- JOIN DOCUMENTS -----------------------------------

bool ColoUiCreator::joinCuiFiles(QString masterFile, QString outputFile, QString workingDir){


    QFile input(masterFile);
    if (!input.open(QFile::ReadOnly)){
        error.error = "Could not open masterFile " +input.fileName() + " for reading";
        error.line = 0;
        return false;
    }

    // Reading all the data
    QTextStream reader(&input);
    QStringList data;
    while (!reader.atEnd()){
        data << reader.readLine();
    }
    input.close();

    //qDebug() << "Number of lines in master file" << data.size();

    qint32 masterIndex = 0;
    QVector<qint32> lcounters;
    QStringList currentFile;

    QStringList includedAlready;
    includedAlready << masterFile;

    lcounters << 0;
    currentFile << masterFile;    

    while (masterIndex < data.size()){

        QString line = data.at(masterIndex);

        masterIndex ++;
        lcounters[lcounters.size()-1]++;

        line = line.trimmed();
        //qDebug() << "Checking for includes: " << line;
        if (line.startsWith(CUI_LANG_INCLUDE)){

            //qDebug() << "Found include " << line;

            // Getting the file.
            QStringList parts = line.split(" ",QString::SkipEmptyParts);
            if (parts.size() < 2){
                error.error = "INCLUDE keyword must be followed by a file path. In file " + currentFile.last();
                error.line = lcounters.last();
                return false;
            }

            // The second part of the line.
            QString newFile = parts.at(1);

            if (!newFile.endsWith(".cui")){
                newFile = newFile + ".cui";
            }

            // Adding the tag that this is from another file
            QFile file(workingDir + "/" + newFile);
            data[masterIndex-1] = CHANGE_FILE_START_SEQUENCE + " " + file.fileName();

            if (includedAlready.contains(file.fileName())){
                error.error = "Attempting to include same file twice: " + file.fileName() +  ". Second time found In file " + currentFile.last();
                error.line = lcounters.last();
                return false;
            }
            if (!file.exists()){
                error.error = "INCLUDE file " + file.fileName() + " does not exist. In file " + currentFile.last();
                error.line = lcounters.last();
                return false;
            }

            if (!file.open(QFile::ReadOnly)){
                error.error = "Could not open INCLUDE file " + file.fileName() + " for reading. In file " + currentFile.last();
                error.line = lcounters.last();
                return false;
            }


            lcounters << 0;           // "Adding a new index"
            currentFile << newFile;   // "Data analyzed from now on belongs to this file"

            QStringList pre = data.mid(0,masterIndex+1);
            QStringList post = data.mid(masterIndex+1);
            data.clear();

            // Reading all data
            QTextStream reader(&file);
            while (!reader.atEnd()){
                pre << reader.readLine();
            }
            file.close();

            // Putting all data together again.
            data << pre << CHANGE_FILE_END_SEQUENCE << post;

            //qDebug() << "Found include. Data size " << data.size();
            includedAlready << file.fileName();

        }
        else if (line.startsWith(CHANGE_FILE_END_SEQUENCE)){
            lcounters.removeLast();
            currentFile.removeLast();
        }

    }

    QFile output(outputFile);
    if (!output.open(QFile::WriteOnly)){
        error.error = "Could not open joined file " + output.fileName();
        error.line = 0;
        return false;        
    }

    QTextStream writer(&output);

    for (qint32 i = 0; i < data.size(); i++){
        writer << data.at(i) << "\n";
    }

    output.close();

    return true;

}


