#include "colouitext.h"

//ColoUiTextInputDialog ColoUiText::inputDialog;

ColoUiText::ColoUiText(QString name, ColoUiTextInputDialog *diag, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    this->type = CUI_TEXT;
    inputDialog = diag;
    this->setFlag(QGraphicsItem::ItemClipsToShape);
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    yDisplacement = 0;
}

void ColoUiText::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
    if (c.getBool(CPR_USE_HTML)){
        config.set(CPR_READ_ONLY,true); // HTML text is read only one way or the other.
    }
    yDisplacement = 0;
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
        res = painter->boundingRect(textBoundingBox,config.getString(CPR_TEXT));
    }

    // Limiting the y Displacement
    if (res.height() > this->h){
        qreal maxy = res.height() - this->h;
        if (yDisplacement > maxy) yDisplacement = maxy;
        else if (yDisplacement < 0) yDisplacement = 0;
        textBoundingBox.setTop(-yDisplacement);
    }

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
        painter->drawText(textBoundingBox,config.getString(CPR_TEXT));
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
    if (config.getBool(CPR_READ_ONLY)){
        // This is scrollable
        QPointF now = mapToScene(e->pos());
        yLastScrollPoint = now.y();
    }
    else{
        if (inputDialog != NULL){
            inputDialog->setInput(config.getString(CPR_TEXT));
            if (inputDialog->exec()){
                config.set(CPR_TEXT,inputDialog->getInput());
                update();
            }
        }
        QGraphicsItem::mousePressEvent(e);
    }
}

void ColoUiText::wheelEvent(QGraphicsSceneWheelEvent *e){
    // A single mouwheel incremente should be about 20% of the height
    qreal dy = this->h*0.2;
    if (e->delta() > 0){
        dy = -dy;
    }
    yDisplacement = yDisplacement + dy;
    update();
}

void ColoUiText::mouseMoveEvent(QGraphicsSceneMouseEvent *e){

    QPointF now = mapToScene(e->pos());
    qreal dy = yLastScrollPoint - now.y();
    yLastScrollPoint = now.y();
    yDisplacement = yDisplacement + dy;

    QGraphicsItem::mouseMoveEvent(e);
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
    updateTextBoundingBox();

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
    updateTextBoundingBox();

}

void ColoUiText::appendText(QString text){
    QString ptext = config.getString(CPR_TEXT);
    ptext = ptext + text;
    config.set(CPR_TEXT,ptext);
    updateTextBoundingBox();

}

void ColoUiText::setText(QString text){
    config.set(CPR_TEXT,text);
    updateTextBoundingBox();
}

void ColoUiText::clearText(){
    setText("");
    updateTextBoundingBox();
}
