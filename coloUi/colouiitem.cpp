#include "colouiitem.h"

ColoUiItem::ColoUiItem(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    currentState = IS_NORMAL;
}

void ColoUiItem::drawItem(QPainter *painter, ItemState state){
    // Calling it with this function means that the bounding box should be recomputed
    boundingBox = QRectF(config.getInt32(CPR_X),config.getInt32(CPR_Y),config.getUInt16(CPR_WIDTH),config.getUInt16(CPR_HEIGHT));
    yIcon = yIcon + config.getInt32(CPR_Y);
    xIcon = xIcon + config.getInt32(CPR_X);    
    currentState = state;

    updateDrawBox();

    this->paint(painter,NULL,NULL);
}

void ColoUiItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Painter set up to draw the shape
    QPen pen;
    pen.setWidth(config.getUInt16(CPR_BORDER_WIDTH));
    pen.setColor(QColor(config.getColor(CPR_BORDER_COLOR)));

    QVariantHash gradient;

    switch (currentState){
    case IS_NORMAL:
        gradient = config.getGradient(CPR_BACKGROUND_COLOR);
        break;
    case IS_ALTERNATIVE:
        gradient = config.getGradient(CPR_ALTERNATIVE_BACKGROUND_COLOR);
        break;
    case IS_HOVER:
        gradient = config.getGradient(CPR_HOVER_BACKGROUND);
        break;
    }

    QBrush brush = ColoUiConfiguration::configureBrushForGradient(gradient,boundingBox);
    painter->setBrush(brush);
    painter->setPen(pen);


    switch (config.getUInt16(CPR_SHAPE)){
    case CPA_ELLIPSE:
        painter->drawEllipse(drawBox);
        break;
    case CPA_ROUND_RECT:
        painter->drawRoundedRect(drawBox,
                                 config.getUInt16(CPR_ROUNDED_RECT_RADIOUS),
                                 config.getUInt16(CPR_ROUNDED_RECT_RADIOUS));
        break;
    default:
        painter->drawRect(drawBox);
        break;
    }


    if (useIcon){
        painter->drawImage(QPointF(xIcon,yIcon),normalIcon);
    }

    pen.setWidth(0);
    if (currentState == IS_ALTERNATIVE){
        painter->setPen(QPen(QColor(config.getColor(CPR_ALTERNATIVE_TEXT_COLOR))));
    }
    else{
        painter->setPen(QPen(QColor(config.getColor(CPR_TEXT_COLOR))));
    }
    painter->setFont(config.getFont());    
    textBoundingBox.moveTo(xText+config.getInt32(CPR_X),yText+config.getInt32(CPR_Y));
    //qDebug() << "Text BB" << textBoundingBox << boundingBox << "for " << config.getString(CPR_TEXT);
    painter->drawText(textBoundingBox,Qt::AlignCenter,config.getString(CPR_TEXT));
}

void ColoUiItem::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);

    // Calculating the text dimensions
    QFontMetrics metrics(config.getFont());
    textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),Qt::AlignCenter,config.getString(CPR_TEXT));
    textBoundingBox.moveTo(0,0);
    useIcon = false;

    //qDebug() << "FM text box" << textBoundingBox;

    if (config.getString(CPR_ICON_PATH).isEmpty()){
        centerTextOnButton();
    }
    else{

        QImage icon(config.getString(CPR_ICON_PATH));

        if (!icon.isNull()){

            useIcon = true;

            if (config.getString(CPR_TEXT).isEmpty()){

                qreal imagew = 0.8*boundingRect().width();
                qreal imageh = 0.8*boundingRect().height();
                normalIcon = icon.scaled(imagew,imageh,Qt::KeepAspectRatio);

                // Center the image in the button.
                xIcon = (boundingBox.width() - normalIcon.width())/2.0;
                yIcon = (boundingBox.height() - normalIcon.height())/2.0;

            }
            else{

                switch (config.getUInt16(CPR_ICON_POSITION)){
                case CPA_UP:
                    configForIconAbove(icon);
                    break;
                case CPA_DOWN:
                    configForIconBelow(icon);
                    break;
                case CPA_LEFT:
                    configForIconLeft(icon);
                    break;
                case CPA_RIGHT:
                    configForIconRight(icon);
                    break;
                }

            }
        }
        else{
            // Same as emtpy icon
            centerTextOnButton();
        }
    }

}

void ColoUiItem::centerTextOnButton(){
    yText = config.getInt32(CPR_Y_OFFSET);
    xText = config.getInt32(CPR_X_OFFSET);
    textBoundingBox = boundingBox;
}

void ColoUiItem::updateDrawBox(){
    qreal airX = (qreal)config.getUInt16(CPR_AIR_X)/100.0;
    qreal airY = (qreal)config.getUInt16(CPR_AIR_Y)/100.0;

    drawBox = QRectF(boundingBox.width()*airX + boundingBox.x(),
                     boundingBox.height()*airY + boundingBox.y(),
                     boundingBox.width()*(1-2*airX),
                     boundingBox.height()*(1-2*airY));
}


//---------------- CONFIGURE VARIABLES FOR DIFFERENT ICON POSITIONS -------------------------------------

void ColoUiItem::configForIconAbove(QImage icon){

    qreal airX = (qreal)config.getUInt16(CPR_AIR_X)/100.0;
    qreal airY = (qreal)config.getUInt16(CPR_AIR_Y)/100.0;
    qreal a = airY*boundingBox.height();
    qreal textOccupies = textBoundingBox.height()+a+config.getInt32(CPR_SPACE_ICON_TEXT);

    normalIcon = icon.scaledToWidth(boundingBox.height()-textOccupies);
    // Checking it fits
    if (normalIcon.width() > boundingBox.width()){
        normalIcon = icon.scaledToHeight(boundingBox.width()*(1-2*airX));
    }

    qreal th = textOccupies + normalIcon.height();
    xIcon = (boundingBox.width() - normalIcon.width())/2;
    yIcon = qMax(a,(boundingBox.height() - th)/2);
    yText = yIcon + normalIcon.height() + a +config.getInt32(CPR_SPACE_ICON_TEXT) + config.getInt32(CPR_Y_OFFSET);
    xText = config.getInt32(CPR_X_OFFSET) + (boundingBox.width() - textBoundingBox.width())/2;
}

void ColoUiItem::configForIconBelow(QImage icon){

    qreal airX = (qreal)config.getUInt16(CPR_AIR_X)/100.0;
    qreal airY = (qreal)config.getUInt16(CPR_AIR_Y)/100.0;
    qreal a = airY*boundingBox.height();
    qreal textOccupies = textBoundingBox.height()+a+config.getInt32(CPR_SPACE_ICON_TEXT);

    normalIcon = icon.scaledToWidth(boundingBox.height()-textOccupies);
    // Checking it fits
    if (normalIcon.width() > boundingBox.width()){
        normalIcon = icon.scaledToHeight(boundingBox.width()*(1-2*airX));
    }

    qreal th = textOccupies + normalIcon.height();
    yText = qMax(a,(boundingBox.height() - th)/2) + config.getInt32(CPR_Y_OFFSET);
    xText = config.getInt32(CPR_X_OFFSET) + (boundingBox.width() - textBoundingBox.width())/2;
    xIcon = (boundingBox.width() - normalIcon.width())/2;
    yIcon = yText + textOccupies;

}

void ColoUiItem::configForIconLeft(QImage icon){

    qreal airY = (qreal)config.getUInt16(CPR_AIR_Y)/100.0;
    qreal airX = (qreal)config.getUInt16(CPR_AIR_X)/100.0;
    qreal a = airX*boundingBox.width();
    qreal textOccupies = textBoundingBox.width()+a+config.getInt32(CPR_SPACE_ICON_TEXT);

    normalIcon = icon.scaledToWidth(boundingBox.width()-textOccupies);
    // Checking it fits
    if (normalIcon.height() > boundingBox.height()){
        normalIcon = icon.scaledToHeight(boundingBox.height()*(1-2*airY));
    }

    qreal tw = textOccupies + normalIcon.width();

    xIcon = qMax(a,(boundingBox.width() - tw)/2);
    yIcon = (boundingBox.height() - normalIcon.height())/2;
    yText = config.getInt32(CPR_Y_OFFSET) + (boundingBox.height() - textBoundingBox.height())/2;
    xText = normalIcon.width() + xIcon + config.getInt32(CPR_X_OFFSET) + config.getInt32(CPR_SPACE_ICON_TEXT);

}


void ColoUiItem::configForIconRight(QImage icon){

    qreal airY = (qreal)config.getUInt16(CPR_AIR_Y)/100.0;
    qreal airX = (qreal)config.getUInt16(CPR_AIR_X)/100.0;
    qreal a = airX*boundingBox.width();
    qreal textOccupies = textBoundingBox.width()+a+config.getInt32(CPR_SPACE_ICON_TEXT);

    normalIcon = icon.scaledToWidth(boundingBox.width()-textOccupies);
    // Checking it fits
    if (normalIcon.height() > boundingBox.height()){
        normalIcon = icon.scaledToHeight(boundingBox.height()*(1-2*airY));
    }

    qreal tw = textOccupies + normalIcon.width();

    yText = config.getInt32(CPR_Y_OFFSET) + (boundingBox.height() - textBoundingBox.height())/2;
    xText = qMax(a,(boundingBox.width() - tw)/2);
    xIcon = xText + textOccupies + config.getInt32(CPR_X_OFFSET);
    yIcon = (boundingBox.height() - normalIcon.height())/2;
}
