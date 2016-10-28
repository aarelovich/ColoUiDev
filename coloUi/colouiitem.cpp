#include "colouiitem.h"

ColoUiItem::ColoUiItem(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    currentState = IS_NORMAL;
}

void ColoUiItem::drawItem(QPainter *painter, ItemState state){
    // Calling it with this function means that the bounding box should be recomputed
    boundingBox = QRectF(config.getInt32(CPR_X),config.getInt32(CPR_Y),config.getUInt16(CPR_WIDTH),config.getUInt16(CPR_HEIGHT));
    yText = yText + config.getInt32(CPR_Y);
    yIcon = yIcon + config.getInt32(CPR_Y);
    xText = xText + config.getInt32(CPR_X);
    xIcon = xIcon + config.getInt32(CPR_X);    
    currentState = state;
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
        painter->drawEllipse(boundingBox);
        break;
    case CPA_ROUND_RECT:
        painter->drawRoundedRect(boundingBox,
                                 config.getUInt16(CPR_ROUNDED_RECT_RADIOUS),
                                 config.getUInt16(CPR_ROUNDED_RECT_RADIOUS));
        break;
    default:
        painter->drawRect(boundingBox);
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
    painter->drawText(QPointF(xText,yText),config.getString(CPR_TEXT));

}

void ColoUiItem::setConfiguration(ColoUiConfiguration c){
    ColoUiElement::setConfiguration(c);

    useIcon = false;
    if (config.getString(CPR_ICON_PATH).isEmpty()){
        qint32 textFlags = Qt::AlignCenter;
        QFontMetrics metrics(config.getFont());
        QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                      textFlags,config.getString(CPR_TEXT));
        yText = textBoundingBox.top() + textBoundingBox.height() + config.getInt32(CPR_Y_OFFSET);
        //qDebug() << "Y Text for" << c.getString(CPR_TEXT) << yText;
        xText = textBoundingBox.left() + config.getInt32(CPR_X_OFFSET);
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
            qint32 textFlags = Qt::AlignCenter;
            QFontMetrics metrics(config.getFont());
            QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                          textFlags,config.getString(CPR_TEXT));
            yText = textBoundingBox.top() + textBoundingBox.height() + config.getInt32(CPR_Y_OFFSET);
            xText = textBoundingBox.left() + config.getInt32(CPR_X_OFFSET);
        }
    }

}


//---------------- CONFIGURE VARIABLES FOR DIFFERENT ICON POSITIONS -------------------------------------

void ColoUiItem::configForIconAbove(QImage icon){
    qint32 textFlags = Qt::AlignHCenter|Qt::AlignBottom;
    QFontMetrics metrics(config.getFont());
    QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                  textFlags,config.getString(CPR_TEXT));

    qreal tempY = textBoundingBox.top() -0.05*boundingBox.height() + config.getInt32(CPR_Y_OFFSET);
    yText = tempY + textBoundingBox.height();
    xText = textBoundingBox.left() + config.getInt32(CPR_X_OFFSET);
    normalIcon = icon.scaledToHeight(0.9*tempY);

    // Checking it fits
    if (normalIcon.width() > boundingBox.width()){
        normalIcon = icon.scaledToWidth(boundingBox.width()*0.95);
    }

    yIcon = (tempY - normalIcon.height())/2.0;
    xIcon = (boundingBox.width() - normalIcon.width())/2.0;
}

void ColoUiItem::configForIconBelow(QImage icon){
    qint32 textFlags = Qt::AlignHCenter|Qt::AlignTop;
    QFontMetrics metrics(config.getFont());
    QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                  textFlags,config.getString(CPR_TEXT));

    qreal tempY = textBoundingBox.top() + 0.05*boundingBox.height() + config.getInt32(CPR_Y_OFFSET);
    yText = tempY + textBoundingBox.height();
    xText = textBoundingBox.left() + config.getInt32(CPR_X_OFFSET);
    normalIcon = icon.scaledToHeight(0.9*(boundingBox.height() - yText));

    // Checking it fits
    if (normalIcon.width() > boundingBox.width()){
        normalIcon = icon.scaledToWidth(boundingBox.width()*0.95);
    }

    xIcon = (boundingBox.width() - normalIcon.width())/2.0;
    yIcon = yText + (boundingBox.height() - yText - normalIcon.height())/2.0;
}

void ColoUiItem::configForIconLeft(QImage icon){
    qint32 textFlags = Qt::AlignVCenter|Qt::AlignRight;
    QFontMetrics metrics(config.getFont());
    QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                  textFlags,config.getString(CPR_TEXT));

    yText = textBoundingBox.top() + textBoundingBox.height() + config.getInt32(CPR_Y_OFFSET);
    xText = textBoundingBox.left()-0.05*boundingBox.width() + config.getInt32(CPR_X_OFFSET);
    normalIcon = icon.scaledToWidth(xText*0.9);

    // Checking it fits
    if (normalIcon.height() > boundingBox.height()){
        normalIcon = icon.scaledToHeight(boundingBox.height()*0.95);
    }

    xIcon = (xText - normalIcon.width())/2;
    yIcon = (boundingBox.height() - normalIcon.height())/2.0;
}


void ColoUiItem::configForIconRight(QImage icon){
    qint32 textFlags = Qt::AlignVCenter|Qt::AlignLeft;
    QFontMetrics metrics(config.getFont());
    QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                  textFlags,config.getString(CPR_TEXT));

    yText = textBoundingBox.top() + textBoundingBox.height() + config.getInt32(CPR_Y_OFFSET);
    xText = textBoundingBox.left()+0.05*boundingBox.width() + config.getInt32(CPR_X_OFFSET);
    normalIcon = icon.scaledToWidth((boundingBox.width() -xText)*0.9);

    // Checking if the icon fits
    if (normalIcon.height() > boundingBox.height()){
        normalIcon = icon.scaledToHeight(boundingBox.height()*0.95);
    }

    xIcon = xText + textBoundingBox.width() + (boundingBox.width() - xText - textBoundingBox.width() - normalIcon.width())/2.0;
    yIcon = (boundingBox.height() - normalIcon.height())/2.0;
}
