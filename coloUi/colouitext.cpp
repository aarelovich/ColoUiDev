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

//    f.setPointSize(config.getUInt16(CPR_FONT_SIZE));
//    painter->setFont(f);
//    QPen pen;

//    painter->setBrush(QBrush(config.backgroundColor));
//    pen.setWidth(config.borderWidth);
//    pen.setColor(config.borderColor);
//    painter->setPen(pen);
//    painter->drawRect(0,0,this->w,this->h);

//    pen.setColor(config.textColor);
//    painter->setBrush(QBrush(config.textColor));
//    painter->setPen(pen);

//    QTextDocument td;
//    td.setHtml("<p style='color:#FF0000;font-size:120px' align='center'>This is some red text</p>");
//    QAbstractTextDocumentLayout::PaintContext ctx;
//    ctx.clip = boundingBox;
//    td.documentLayout()->draw(painter,ctx);

    //painter->drawText(boundingBox,config.text);

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

