#include "colouitext.h"

//ColoUiTextInputDialog ColoUiText::inputDialog;

ColoUiText::ColoUiText(QString name, ColoUiTextInputDialog *diag, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    this->type = CUI_TEXT;
    inputDialog = diag;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    yDisplacement = 0;
    deltaY = 0;
    acceptedInput = QRegExp("[\\w\\+\\-\\.~!@#$%\\^&\\*\\(\\);:\\\\\\/\\|\\<\\>\\\"\\'?,\\{\\}\\[\\]]");
}

void ColoUiText::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
    if (c.getBool(CPR_USE_HTML)){
        config.set(CPR_READ_ONLY,true); // HTML text is read only one way or the other.
    }
    else{
        textAnalyzer.setFont(config.getFont());
        if (textAnalyzer.getText() != config.getString(CPR_TEXT)){
            textAnalyzer.setText(config.getString(CPR_TEXT));
        }
        editingEnabled = false;
    }
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
}

void ColoUiText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
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
    brush = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_TEXT_COLOR),boundingBox);
    painter->setBrush(brush);

    // Obtaining the bounding box depending on the type
    QRectF res;
    if (config.getBool(CPR_USE_HTML)){
        QGraphicsTextItem text;
        text.setHtml(config.getString(CPR_TEXT));
        text.setTextWidth(textBoundingBox.width());
        res = text.boundingRect();
    }
    else{
        res = painter->boundingRect(textBoundingBox,textAnalyzer.getText());
    }

    qreal maxy = res.height() - this->h;

    // Doing slider logic
    if (movingSlider){
        if (maxy > 0){
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
            pen.setColor(QColor("#ffff00"));
            pen.setWidth(3);
            painter->setPen(pen);
            //qDebug() << "Cursor" << cursor;
            QPointF p1(cursor.x() + config.getUInt16(CPR_X_OFFSET),cursor.y() - yDisplacement + config.getUInt16(CPR_Y_OFFSET));
            QPointF p2 (p1.x(),p1.y() + textAnalyzer.getCharHeight());
            painter->drawLine(p1,p2);
            painter->setPen(oldpen);

        }
        painter->drawText(textBoundingBox,textAnalyzer.getText());
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

void ColoUiText::updateTextBoundingBox(){
    qreal textX = config.getUInt16(CPR_X_OFFSET);
    qreal textW = this->w - textX;
    qreal textH = this->h;
    qreal textY = 0;
    textBoundingBox = QRectF(textX,textY,textW,textH);
    update();
}

void ColoUiText::mousePressEvent(QGraphicsSceneMouseEvent *e){
    // This is scrollable
    QPointF now = mapToScene(e->pos());
    yLastScrollPoint = now.y();

    // Checking if the slider is pressed
    movingSlider = false;
    if (scrollEnabled){
        if (scrollBarWidth > 0){
            // In the scroll bar
            if (e->pos().x() > scrollBarX) {
                // Over the actual slider
                if ((e->pos().y() > sliderPosition) && (e->pos().y() < (sliderHeight + sliderPosition))){
                    movingSlider = true;
                    return;
                }
            }
        }
    }

    editingEnabled = false;

}

void ColoUiText::wheelEvent(QGraphicsSceneWheelEvent *e){
    // A single mouwheel incremente should be about 20% of the height
    qreal dy = this->h*0.2;
    if (e->delta() > 0){
        dy = -dy;
    }
    deltaY = dy;
    update();
}

void ColoUiText::mouseMoveEvent(QGraphicsSceneMouseEvent *e){

    QPointF now = mapToScene(e->pos());
    deltaY = yLastScrollPoint - now.y();
    yLastScrollPoint = now.y();
    movingText = true;
    QGraphicsItem::mouseMoveEvent(e);
    update();

}

void ColoUiText::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e);
    movingSlider = false;

    if (!movingText){
        editingEnabled = true;
        cursor = textAnalyzer.currentCursorPos();
        // TODO raise soft keyboard if NECESSARY
        update();
    }

    movingText = false;

}

void ColoUiText::focusOutEvent(QFocusEvent *e){
    Q_UNUSED(e);
    editingEnabled = false;
    update();
}

// -------------- Key Press Related Functions -----------------
void ColoUiText::keyPressEvent(QKeyEvent *e){
    switch (e->key()){
    case Qt::Key_Return:
        textAnalyzer.newLine();
        break;
    case Qt::Key_Up:
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    case Qt::Key_Down:
        break;
    case Qt::Key_Backspace:
        textAnalyzer.backSpace();
        break;
    case Qt::Key_Space:
        textAnalyzer.appendText(" ");
        break;
    default:
        if (acceptedInput.exactMatch(e->text())){
            textAnalyzer.appendText(e->text());
        }
        break;
    }
    cursor = textAnalyzer.currentCursorPos();
    update();
}

// -------------- Text Realated functions ---------------------
void ColoUiText::appendFormattedText(QString text, QFont font, QColor textcolor){

    if (!config.getBool(CPR_USE_HTML)){
        appendText(text);
        return;
    }

    QString html = "<span style = '";
    html = html + "color:" + textcolor.name() + "; ";
    html = html + "font-family: " + font.family() + "; ";
    html = html + "font-size: " + QString::number(font.pointSize()) + "px; ";
    if (font.bold()){
        html = html + "font-weight: 900; ";
    }
    if (font.italic()){
        html = html  + "font-style: italic; ";
    }
    html = html + "'>" + text + "</span>";

    QString ptext = config.getString(CPR_TEXT);
    ptext = ptext + html;
    config.set(CPR_TEXT,ptext);
    update();

}

void ColoUiText::appendFormattedText(QString text, QFont font, QColor textcolor, QColor background){

    if (!config.getBool(CPR_USE_HTML)){
        appendText(text);
        return;
    }

    QString html = "<span style = '";
    html = html + "color:" + textcolor.name() + "; ";
    html = html + "font-family: " + font.family() + "; ";
    html = html + "font-size: " + QString::number(font.pointSize()) + "px; ";
    html = html + "background-color: " + background.name() + "; ";
    if (font.bold()){
        html = html + "font-weight: 900; ";
    }
    if (font.italic()){
        html = html  + "font-style: italic; ";
    }
    html = html + "'>" + text + "</span>";

    QString ptext = config.getString(CPR_TEXT);
    ptext = ptext + html;
    config.set(CPR_TEXT,ptext);
    update();

}

void ColoUiText::appendText(QString text){
    textAnalyzer.appendText(text);
    update();

}

void ColoUiText::setText(QString text){
    textAnalyzer.setText(text);
    update();
}

void ColoUiText::clearText(){
    textAnalyzer.setText("");
    update();
}


//******************************** TEXT ANALYZER FUNCTIONS ***************************************
TextAnalyzer::TextAnalyzer(){
    linePos = 0;
    colPos = 0;
    yCursor = 0;
    xCursor = 0;
    textLines << "";
}

void TextAnalyzer::setFont(QFont f){
    fm = new QFontMetricsF(f);
    QString test = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QFontMetrics fmetrics = QFontMetrics(f);
    QRectF brect = fmetrics.boundingRect(test);
    charHeight = brect.height();
    charWidth = fmetrics.maxWidth();

}

void TextAnalyzer::setText(QString t){
    textLines = t.split(QRegExp("[\r\n]"));
    linePos = textLines.size()-1;
    if (linePos < 0){
        linePos = 0;
        yCursor = 0;
        textLines.clear();
        textLines << "";
        colPos = 0;
        xCursor = 0;
        return;
    }
    colPos = textLines.at(linePos).size();
    //xCursor = charWidth*colPos;
    xCursor = fm->width(textLines.at(linePos));
    if (colPos < 0){
        colPos = 0;
        xCursor = 0;
    }
}

QString TextAnalyzer::getText() const{
    return textLines.join("\n");
}

void TextAnalyzer::appendText(QString newText){
    QString line = textLines.at(linePos);
    line = line.insert(colPos,newText);
    colPos = colPos + newText.size();
    //xCursor = xCursor  + charWidth;
    xCursor = xCursor + fm->width(newText);
    textLines[linePos] = line;
}

void TextAnalyzer::backSpace(){
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
    QString s = ""; s = s + line.at(line.size()-1);
    line = line.remove(line.size()-1,1);
    colPos--;
    xCursor = xCursor - fm->width(s);
    textLines[linePos] = line;
}

void TextAnalyzer::newLine(){
    colPos = 0;
    xCursor = 0;
    textLines << "";
    linePos = textLines.size() - 1;
    yCursor = yCursor + charHeight;
}


QPointF TextAnalyzer::currentCursorPos(){
    return QPointF(xCursor,yCursor);
}


