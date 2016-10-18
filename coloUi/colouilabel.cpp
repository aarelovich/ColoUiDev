#include "colouilabel.h"

ColoUiLabel::ColoUiLabel(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
}

void ColoUiLabel::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);
}

void ColoUiLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget)
    Q_UNUSED(option)

    QFont f = config.getFont();
    painter->setFont(f);
    QPen pen;

    if (!config.getBool(CPR_DISABLE_BACKGROUND)){
        // Drawing background
        QBrush brush = ColoUiConfiguration::configureBrushForGradient(config.getGradient(CPR_BACKGROUND_COLOR),boundingBox);
        painter->setBrush(brush);
        pen.setWidth(config.getUInt16(CPR_BORDER_WIDTH));
        pen.setColor(QColor(config.getColor(CPR_BORDER_COLOR)));
        painter->setPen(pen);
        painter->drawRect(0,0,this->w,this->h);
    }

    if (config.getBool(CPR_USE_HTML)){
        QTextDocument td;
        td.setHtml(config.getString(CPR_TEXT));
        td.setTextWidth(this->w);
        td.documentLayout()->draw(painter,QAbstractTextDocumentLayout::PaintContext());
    }
    else{
        // Drawing plain text
        painter->setBrush(QBrush(QColor(config.getColor(CPR_TEXT_COLOR))));
        pen.setColor(config.getColor(CPR_TEXT_COLOR));
        pen.setWidth(0);
        painter->setPen(pen);
        painter->drawText(boundingBox,config.getString(CPR_TEXT));
    }


}
