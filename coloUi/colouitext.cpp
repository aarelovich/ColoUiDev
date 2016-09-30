#include "colouitext.h"

//ColoUiTextInputDialog ColoUiText::inputDialog;

ColoUiText::ColoUiText(QString name, ColoUiTextInputDialog *diag, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    inputDialog = diag;
}

void ColoUiText::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
    update();
}

void ColoUiText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget)
    Q_UNUSED(option)
    Q_UNUSED(painter)

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

    if (config.getBool(CPR_USE_HTML)){
        QTextDocument td;
        td.setHtml(config.getString(CPR_TEXT));
        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.clip = boundingBox;
        td.documentLayout()->draw(painter,ctx);
    }
    else{
        painter->drawText(boundingBox,config.getString(CPR_TEXT));
    }

}

void ColoUiText::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if (config.getBool(CPR_READ_ONLY)) return;
    if (inputDialog != NULL){
        inputDialog->setInput(config.getString(CPR_TEXT));
        if (inputDialog->exec()){
            config.set(CPR_TEXT,inputDialog->getInput());
            update();
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

