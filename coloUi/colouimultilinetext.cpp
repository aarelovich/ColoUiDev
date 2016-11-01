#include "colouimultilinetext.h"


ColoUiMultiLineText::ColoUiMultiLineText(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    this->type = CUI_MULTILINE_TEXT;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    yDisplacement = 0;
    deltaY = 0;
    acceptedInput = QRegExp("[\\w\\+\\-\\.~!@#$%=_\\^&\\*\\(\\);:\\\\\\/\\|\\<\\>\\\"\\'?,\\{\\}\\[\\]]");
}

void ColoUiMultiLineText::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
    if (c.getBool(CPR_USE_HTML)){
        config.set(CPR_READ_ONLY,true); // HTML text is read only one way or the other.        
    }
    else{
        textManager.configureAnalyzer(config.getFont(),this->w - config.getUInt16(CPR_X_OFFSET),this->h);
        if (textManager.getText() != config.getString(CPR_TEXT)){
            textManager.setText(config.getString(CPR_TEXT));
        }
    }
    editingEnabled = false;
    yDisplacement = 0;

    if (config.getBool(CPR_V_SCROLLBAR)){
        scrollBarWidth = 0.02*(qreal)this->w;
        scrollBarX = this->w-scrollBarWidth;
    }
    else{
        scrollBarWidth = 0;
        scrollBarX = this->w;
    }

    // Calculating scrollbar variables.
    sliderHeight = 0.05*this->h;
    endScrollBarPoint = this->h - sliderHeight;
    sliderPosition = 0;
    scrollEnabled = true;

    updateTextBoundingBox();

    setText(config.getString(CPR_TEXT));

    if (!config.getBool(CPR_USE_HTML)){
        if (!config.getBool(CPR_READ_ONLY)){
            this->setCursor(QCursor(Qt::IBeamCursor));
        }
        else{
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    else{
        this->setCursor(QCursor(Qt::ArrowCursor));
    }

    movingSlider = false;

}

void ColoUiMultiLineText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget)
    Q_UNUSED(option)

    QFont f = config.getFont();
    painter->setFont(f);
    QPen pen;

    // Drawing background    
    QBrush brush = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_BACKGROUND_COLOR),boundingBox);
    painter->setBrush(brush);

    pen.setWidth(config.getUInt16(CPR_BORDER_WIDTH));
    pen.setColor(QColor(config.getColor(CPR_BORDER_COLOR)));

    painter->setPen(pen);
    painter->drawRect(0,0,this->w,this->h);

    // Drawing text
    painter->setBrush(QBrush(QColor(config.getColor(CPR_TEXT_COLOR))));

    // Obtaining the bounding box depending on the type
    QRectF res;
    if (config.getBool(CPR_USE_HTML)){
        QGraphicsTextItem text;
        text.setHtml(config.getString(CPR_TEXT));
        text.setTextWidth(textBoundingBox.width());
        res = text.boundingRect();
    }
    else{
        res = painter->boundingRect(textBoundingBox,textManager.getText());
    }

    // Doing slider and text y displacement logic

    qreal maxy = res.height() - this->h;
    if (movingSlider){
        if (maxy > 0){
            //qWarning() << "Should update sliderPosition by deltaY of" << deltaY;
            yDisplacement = yDisplacement - deltaY*maxy/endScrollBarPoint;
            sliderPosition = sliderPosition - deltaY;
            if (sliderPosition > endScrollBarPoint){
                sliderPosition = endScrollBarPoint;
            }
            else if (sliderPosition < 0){
                sliderPosition = 0;
            }
        }
    }
    else{
        yDisplacement = yDisplacement + deltaY;
        deltaY = 0; // Making sure it is not added again...
        if (scrollBarWidth > 0){
            if (maxy > 0){
                sliderPosition = sliderPosition + endScrollBarPoint*deltaY/maxy;
                if (sliderPosition > endScrollBarPoint){
                    sliderPosition = endScrollBarPoint;
                }
                else if (sliderPosition < 0){
                    sliderPosition = 0;
                }
            }            
        }
    }

    // Limiting the y Displacement and displacing the text bounding box.
    if (maxy > 0){
        if (yDisplacement > maxy) yDisplacement = maxy;
        else if (yDisplacement < 0) yDisplacement = 0;        
    }
    else yDisplacement = 0;    
    textBoundingBox.setTop(-yDisplacement);

    if (config.getBool(CPR_USE_HTML)){
        QTextDocument td;
        td.setHtml(config.getString(CPR_TEXT));
        td.setTextWidth(textBoundingBox.width());
        textBoundingBox.setTop(-yDisplacement);
        // Drawing the rich text
        painter->save();
        painter->translate(textBoundingBox.left(),textBoundingBox.top());
        td.documentLayout()->draw(painter,QAbstractTextDocumentLayout::PaintContext());
        painter->restore();

    }
    else{
        // Drawing plain text
        if (editingEnabled){
            QPen oldpen = painter->pen();
            QPen pen;
            pen.setColor(QColor(config.getColor(CPR_CURSOR_COLOR)));
            pen.setWidth(3);
            painter->setPen(pen);
            QPointF p1(cursor.x() + config.getUInt16(CPR_X_OFFSET),cursor.y() - yDisplacement);
            QPointF p2 (p1.x(),p1.y() + textManager.getCharHeight());
            painter->drawLine(p1,p2);
            painter->setPen(oldpen);

        }
        pen.setColor(config.getColor(CPR_TEXT_COLOR));
        pen.setWidth(0);
        painter->setPen(pen);
        painter->drawText(textBoundingBox,textManager.getText());
    }

    // Drawing the scroll bar
    if (scrollBarWidth > 0){
        QRectF scrollBox(scrollBarX,0,scrollBarWidth,this->h);
        QBrush b = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_SCROLLBAR_BACKGROUND),scrollBox);
        painter->setBrush(b);
        painter->drawRect(scrollBox);

        QRect r(scrollBarX,sliderPosition,scrollBarWidth,sliderHeight);
        if (maxy <= 0){
            sliderPosition = 0;
            r.setHeight(this->h);
        }

        b = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_SCROLL_SLIDER),r);
        painter->setBrush(b);
        painter->drawRect(r);
    }

}

void ColoUiMultiLineText::updateTextBoundingBox(){
    qreal textX = config.getUInt16(CPR_X_OFFSET);
    qreal textW = this->w - textX;
    qreal textH = this->h;
    qreal textY = 0;
    textBoundingBox = QRectF(textX,textY,textW,textH);
    update();
}

void ColoUiMultiLineText::mousePressEvent(QGraphicsSceneMouseEvent *e){
    // This is scrollable

    if (editingEnabled) return;

    QPointF now = mapToScene(e->pos());
    yLastScrollPoint = now.y();

    // Checking if the slider is pressed
    movingSlider = false;
    // qWarning() << "Inside the scroll bar" << e->pos() << sliderPosition << sliderHeight << scrollEnabled << scrollBarWidth << scrollBarX;
    if (scrollEnabled){
        if (scrollBarWidth > 0){
            // In the scroll bar
            if (e->pos().x() > scrollBarX) {                
                // Over the actual slider
                if ((e->pos().y() > sliderPosition) && (e->pos().y() < (sliderHeight + sliderPosition))){
                    //qWarning() << "In the slider";
                    //qWarning() << "Pressed";
                    movingSlider = true;
                    return;
                }
            }
        }
    }

}

void ColoUiMultiLineText::wheelEvent(QGraphicsSceneWheelEvent *e){

    // A single mouwheel incremente should be about 20% of the height
    qreal dy = this->h*0.2;
    if (e->delta() > 0){
        dy = -dy;
    }
    deltaY = dy;
    update();

}

void ColoUiMultiLineText::mouseMoveEvent(QGraphicsSceneMouseEvent *e){

    //qDebug() << "Moving mouse with movingSlider" << movingSlider;

    QPointF now = mapToScene(e->pos());
    deltaY = yLastScrollPoint - now.y();
    yLastScrollPoint = now.y();
    if (!movingSlider){
        if (qAbs(deltaY) < SCREEN_HEIGHT/100.0){
            deltaY = 0;
        }
        else movingText = true;
    }
    //qDebug() << "Moving slider" << movingSlider << "DY"<< deltaY;
    QGraphicsItem::mouseMoveEvent(e);    
    update();

}

void ColoUiMultiLineText::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e);
    movingSlider = false;

    if (!movingText){

        //qWarning() << "not moving text";

        if (!config.getBool(CPR_USE_HTML) && !config.getBool(CPR_READ_ONLY)){

            //qWarning() << "is editable";

            if (config.getBool(CPR_USE_VIRTUAL_KEYBOARD)){

                //qWarning() << "will use v keyboard";

                if (!virtualKeyboardInUse){

                    //qWarning() << "calling v keyboard";

                    editingEnabled = true;
                    virtualKeyboardInUse = true;
                    cursor = textManager.setCursorPosition(e->pos(),yDisplacement);
                    signalInfo.type = ST_KEYBOARD_REQUEST;
                    signalSender->sendSignal(signalInfo);
                    update();
                }
            }
            else{
                editingEnabled = true;
                cursor = textManager.setCursorPosition(e->pos(),yDisplacement);
                update();
            }

        }
    }

    movingText = false;

}

void ColoUiMultiLineText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e);
    if ((config.getBool(CPR_USE_HTML) || config.getBool(CPR_READ_ONLY))){
        qDebug() << "Double clicked";
        signalInfo.type = ST_MOUSE_DOUBLE_CLICK;
        signalSender->sendSignal(signalInfo);
    }
}

void ColoUiMultiLineText::focusOutEvent(QFocusEvent *e){
    Q_UNUSED(e);
    if (!config.getBool(CPR_USE_VIRTUAL_KEYBOARD)){
        editingEnabled = false;
        signalInfo.type = ST_TEXT_CHANGED;
        signalSender->sendSignal(signalInfo);
        update();
    }
}

// -------------- Key Press Related Functions -----------------

void ColoUiMultiLineText::softKeyboardInterface(ColoUiKeyType kt, QString data){
    QKeyEvent *e = ColoUiElement::createKeyEvent(kt,data);
    if (kt == KT_HIDE){
        signalInfo.type = ST_KEYBOARD_HIDE;
        signalSender->sendSignal(signalInfo);
        editingEnabled = false;
        update();
    }
    else{
        editingEnabled = true;
        this->keyPressEvent(e);
    }
}

void ColoUiMultiLineText::keyPressEvent(QKeyEvent *e){

    // The only global exception is appending a text file
    if (e->modifiers() == Qt::ControlModifier){
        if (e->key() == Qt::Key_C){
            QApplication::clipboard()->setText(textManager.getText());
            return;
        }
    }

    if (!editingEnabled) return;

    if (e->modifiers() == Qt::ControlModifier){
        if (e->key() == Qt::Key_V){
            textManager.copyText(QApplication::clipboard()->text());
            update();
            return;
        }
    }

    qreal yOffset = -yDisplacement;

    switch (e->key()){
    case Qt::Key_Return:
        deltaY = textManager.newLine(yOffset);
        break;
    case Qt::Key_Up:
        deltaY = textManager.arrowMoveCursor(CPA_UP,yOffset);
        break;
    case Qt::Key_Left:
        textManager.arrowMoveCursor(CPA_LEFT,0);
        break;
    case Qt::Key_Right:
        textManager.arrowMoveCursor(CPA_RIGHT,0);
        break;
    case Qt::Key_Down:
        deltaY = textManager.arrowMoveCursor(CPA_DOWN, yOffset);
        break;
    case Qt::Key_Backspace:
        textManager.backSpace();
        break;
    case Qt::Key_Home:
        textManager.goToLinePos(true);
        break;
    case Qt::Key_End:
        textManager.goToLinePos(false);
        break;
    case Qt::Key_Space:
        textManager.appendText(" ");
        break;
    default:        
        if (acceptedInput.exactMatch(e->text())){
            textManager.appendText(e->text());
        }
        break;
    }
    cursor = textManager.currentCursorPos();
    update();
}

// -------------- Text Realated functions ---------------------
void ColoUiMultiLineText::appendFormattedText(QString text, QFont font, QColor textcolor, bool appendNewLine){

    if (!config.getBool(CPR_USE_HTML)){
        appendText(text);
        return;
    }

    QString html = "<span style = '";
    html = html + "color:" + textcolor.name() + "; ";
    html = html + "font-family: " + font.family() + "; ";
    html = html + "font-size: " + QString::number(font.pixelSize()) + "px; ";
    if (font.bold()){
        html = html + "font-weight: 900; ";
    }
    if (font.italic()){
        html = html  + "font-style: italic; ";
    }
    html = html + "'>" + text + "</span>";

    if (appendNewLine){
        html = html + "<br>";
    }

    QString ptext = config.getString(CPR_TEXT);
    ptext = ptext + html;
    config.set(CPR_TEXT,ptext);
    update();

}

void ColoUiMultiLineText::appendFormattedText(QString text, QFont font, QColor textcolor, bool appendNewLine, QColor background){

    if (!config.getBool(CPR_USE_HTML)){
        appendText(text);
        return;
    }

    QString html = "<span style = '";
    html = html + "color:" + textcolor.name() + "; ";
    html = html + "font-family: " + font.family() + "; ";
    html = html + "font-size: " + QString::number(font.pixelSize()) + "px; ";
    html = html + "background-color: " + background.name() + "; ";
    if (font.bold()){
        html = html + "font-weight: 900; ";
    }
    if (font.italic()){
        html = html  + "font-style: italic; ";
    }
    html = html + "'>" + text + "</span>";

    if (appendNewLine){
        html = html + "<br>";
    }

    QString ptext = config.getString(CPR_TEXT);
    ptext = ptext + html;
    config.set(CPR_TEXT,ptext);
    update();

}

void ColoUiMultiLineText::appendText(QString text){
    textManager.appendText(text);
    update();

}

void ColoUiMultiLineText::setText(QString text){
    textManager.setText(text);
    update();
}

void ColoUiMultiLineText::clearText(){
    textManager.setText("");
    config.set(CPR_TEXT,"");
    update();
}


//******************************** TEXT ANALYZER FUNCTIONS ***************************************
TextManager::TextManager(){
    linePos = 0;
    colPos = 0;
    yCursor = 0;
    xCursor = 0;
    textLines << "";
    fm = nullptr;
}

void TextManager::configureAnalyzer(QFont f, qreal parentW, qreal parentH){
    if (fm != nullptr){
        delete fm;
    }
    fm = new QFontMetricsF(f);
    charHeight = fm->leading() + fm->height();
    maxLineWidth = parentW;
    maxHeight = parentH;
}

void TextManager::setText(QString t){
    textLines.clear();
    copyText(t);
}

QString TextManager::getText() const{
    return textLines.join("\n");
}

void TextManager::copyText(const QString &t){
    QString line = "";
    QString word = "";

    if (textLines.size() == 1){
        if (textLines.front().isEmpty()){
            textLines.clear();
        }
    }

    for (qint32 i = 0; i < t.size(); i++){

        //qDebug() << "Analyzing |" << t.at(i) << "|";

        if (t.at(i) == '\n'){
            line = line + " " + word;
            //qDebug() << "Found newline appending" << line;
            textLines << line;
            line = "";
            word = "";
        }
        else if (t.at(i) == ' '){
            if (line.isEmpty()) line = word;
            else line = line + ' ' + word;
            word = "";
        }
        else{
            word = word + t.at(i);
            if (fm->width(line) + fm->width(word) > maxLineWidth){
                textLines << line;
                line = "";
            }
        }
    }

    textLines << line + " " + word;

    linePos = 0;
    yCursor = 0;
    colPos = 0;
    xCursor = 0;
}

void TextManager::appendText(QString newText){

    if (fm->width(textLines.at(linePos)) + fm->maxWidth() >= maxLineWidth) return;

    QString line = textLines.at(linePos);
    line = line.insert(colPos,newText);
    colPos = colPos + newText.size();
    xCursor = xCursor + fm->width(newText);
    textLines[linePos] = line;
}

void TextManager::backSpace(){
    QString line = textLines.at(linePos);
    if (line.isEmpty()){
        if (linePos == 0) return; // Nothing to do.
        textLines.removeAt(linePos);
        linePos--;
        yCursor = yCursor - charHeight;
        if (linePos < 0){
            colPos = 0;
            xCursor = 0;
            linePos = 0;
            yCursor = 0;
        }
        else{
            QString line = textLines.at(linePos);
            colPos = line.size(); // End position
            xCursor = fm->width(textLines.at(linePos));
        }
        return;
    }
    if (colPos == 0) return; // Cannot delete new line, unless line is empty!!
    line.remove(colPos-1,1);
    colPos--;
    xCursor = fm->width(line.mid(0,colPos));
    textLines[linePos] = line;
}

qreal TextManager::newLine(qreal yOffset){

    QString line = textLines.at(linePos);
    QString preLine, postLine;
    if (colPos == line.size()){
        preLine = line;
        postLine = "";
    }
    else if (colPos == 0){
        preLine = "";
        postLine = line;
    }
    else{
        preLine = line.mid(0,colPos);
        postLine = line.mid(colPos);
    }

    textLines[linePos] = preLine;
    if (linePos == textLines.size()){
        textLines << postLine;
    }
    else{
        textLines.insert(linePos+1,postLine);
    }
    linePos++;
    colPos = 0;
    xCursor = 0;
    yCursor = yCursor + charHeight;

    if ((yCursor + charHeight + yOffset) > maxHeight){
        return charHeight;
    }
    else return 0;

}

QPointF TextManager::setCursorPosition(QPointF click, qreal yoffset){

    if (textLines.size() == 1){
        if (textLines.first().isEmpty()){
            linePos = 0;
            colPos = 0;
            xCursor = 0;
            yCursor = 0;
            return QPointF(0,0);
        }
    }

    qreal yvalue = click.y() + qAbs(yoffset);
    qreal xvalue = click.x();
    linePos = yvalue/charHeight;

    if (linePos >= textLines.size()){
        linePos = textLines.size()-1;
        colPos = textLines.last().size();
        xCursor = fm->width(textLines.last());
        yCursor = (textLines.size()-1)*charHeight;
        return QPointF(xCursor,yCursor);
    }

    yCursor = linePos*charHeight;
    QString line = textLines.at(linePos);

    colPos = -1;
    for (qint32 i = 0; i <= line.size(); i++){
        qreal w = fm->width(line.mid(0,i));
        if (w > xvalue){
            colPos = i-1;
            break;
        }
    }
    if (colPos < 0){
        colPos = line.size();
    }
    xCursor = fm->width(line.mid(0,colPos));

    return QPointF(xCursor,yCursor);

}

QPointF TextManager::currentCursorPos(){
    return QPointF(xCursor,yCursor);
}

void TextManager::goToLinePos(bool start){
    if (start){
        xCursor = 0;
        colPos = 0;
    }
    else{
        QString line = textLines.at(linePos);
        xCursor = fm->width(line);
        colPos = line.size();
    }
}

qreal TextManager::arrowMoveCursor(qint32 direction, qreal yOffset){

    QString line;
    qreal correction = 0;
    switch (direction){

    case CPA_UP:

        if (linePos > 0){

            linePos--;
            yCursor = yCursor - charHeight;
            //qDebug() << "UP yCursor" << yCursor << "yOffset" << yoffset << "ch" << charHeight;
            if (yCursor+yOffset < 0){
                correction = -charHeight;
            }

            line = textLines.at(linePos);
            if (line.size() < colPos){
                colPos = line.size();
                xCursor = fm->width(line);
            }
        }

        break;
    case CPA_DOWN:
        if (linePos < textLines.size()-1){

            linePos++;
            yCursor = yCursor + charHeight;
            //qDebug() << "DW yCursor" << yCursor << "yOffset" << yoffset << "ch" << charHeight << "h" << maxHeight;
            if ((yCursor + charHeight + yOffset) > maxHeight){
                correction = charHeight;
            }

            line = textLines.at(linePos);
            if (line.size() < colPos){
                colPos = line.size();
                xCursor = fm->width(line);
            }
        }
        break;
    case CPA_RIGHT:
        line = textLines.at(linePos);
        if (colPos < line.size()){
            colPos++;
            xCursor = fm->width(line.mid(0,colPos));
        }
        break;
    case CPA_LEFT:
        line = textLines.at(linePos);
        if (colPos > 0){
            colPos--;
            xCursor = fm->width(line.mid(0,colPos));
        }
        break;
    default:
        break;

    }

    return correction;

}


