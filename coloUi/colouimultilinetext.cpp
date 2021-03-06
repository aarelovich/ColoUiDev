#include "colouimultilinetext.h"


ColoUiMultiLineText::ColoUiMultiLineText(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    this->type = CUI_MULTILINE_TEXT;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    yDisplacement = 0;
    deltaY = 0;
}

void ColoUiMultiLineText::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);

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
    // Obtaining the bounding box depending on the type
    QRectF res;
    if (config.getBool(CPR_USE_HTML)){
        QGraphicsTextItem text;
        text.setHtml(config.getString(CPR_TEXT));
        text.setTextWidth(textBoundingBox.width());
        res = text.boundingRect();
    }
    else{
        res = painter->boundingRect(textBoundingBox,config.getString(CPR_TEXT));
    }

    // Doing slider and text y displacement logic

    qreal maxy = res.height() - this->h + config.getInt32(CPR_Y_OFFSET);
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
        if (scrollBarWidth > 0){
            if (maxy > 0){
                sliderPosition = sliderPosition + endScrollBarPoint*deltaY/maxy;
                //qDebug() << "Slider pos" << sliderPosition << endScrollBarPoint;
                if (sliderPosition > endScrollBarPoint){
                    sliderPosition = endScrollBarPoint;
                }
                else if (sliderPosition < 0){
                    sliderPosition = 0;
                }
            }
        }
        deltaY = 0; // Making sure it is not added again...
    }

    // Limiting the y Displacement and displacing the text bounding box.
    if (maxy > 0){
        if (yDisplacement > maxy) yDisplacement = maxy;
        else if (yDisplacement < 0) yDisplacement = 0;
    }
    else yDisplacement = 0;

    if (config.getBool(CPR_USE_HTML)){
        QTextDocument td;
        td.setHtml(config.getString(CPR_TEXT));
        td.setTextWidth(textBoundingBox.width());
        // Drawing the rich text
        painter->save();
        painter->translate(config.getInt32(CPR_X_OFFSET),config.getUInt16(CPR_Y_OFFSET)-yDisplacement);
        td.documentLayout()->draw(painter,QAbstractTextDocumentLayout::PaintContext());
        painter->restore();
    }
    else{
        // Drawing plain text
        pen.setColor(config.getColor(CPR_TEXT_COLOR));
        pen.setWidth(0);
        painter->setPen(pen);
        res.moveTo(config.getInt32(CPR_X_OFFSET),config.getInt32(CPR_Y_OFFSET)-yDisplacement);
        painter->drawText(res,config.getString(CPR_TEXT));
    }

    pen.setColor(QColor(config.getColor(CPR_BORDER_COLOR)));
    pen.setWidth(0);
    painter->setPen(pen);

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
    qreal textW = this->w - config.getUInt16(CPR_X_OFFSET) - scrollBarWidth;
    qreal textH = (this->h - config.getUInt16(CPR_Y_OFFSET))*20;
    textBoundingBox = QRectF(0,0,textW,textH);
    update();
}

void ColoUiMultiLineText::mousePressEvent(QGraphicsSceneMouseEvent *e){
    // This is scrollable
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
    QGraphicsItem::mouseMoveEvent(e);    
    update();

}

void ColoUiMultiLineText::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e);
    movingSlider = false;
}

void ColoUiMultiLineText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e);
    signalInfo.type = ST_MOUSE_DOUBLE_CLICK;
    signalSender->sendSignal(signalInfo);
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
    text = processTextForLineBreaks(text);
    config.set(CPR_TEXT,config.getString(CPR_TEXT) + text);
    update();

}

void ColoUiMultiLineText::setText(QString text){
    text = processTextForLineBreaks(text);
    config.set(CPR_TEXT,text);
    update();
}

void ColoUiMultiLineText::clearText(){
    config.set(CPR_TEXT,"");
    update();
}


QString ColoUiMultiLineText::processTextForLineBreaks(QString input){
    if (config.getBool(CPR_USE_HTML)) return input;

    QStringList lines = input.split("\\n");
    return lines.join("\n");
}
