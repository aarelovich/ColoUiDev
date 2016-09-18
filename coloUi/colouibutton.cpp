#include "colouibutton.h"

ColoUiButton::ColoUiButton(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    isPressed = false;
    signalInfo.type = ST_MOUSE_CLICK;
}

void ColoUiButton::setConfiguration(ColoUiElementConfig c){
    ColoUiElement::setConfiguration(c);

    config.font.setPointSize(config.fontSize);

    useIcon = false;
    if (config.iconPath.isEmpty()){
        qint32 textFlags = Qt::AlignCenter;
        QFontMetrics metrics(config.font);
        QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                      textFlags,config.text);
        yText = textBoundingBox.top() + textBoundingBox.height() + config.yOffset;
        xText = textBoundingBox.left() + config.xOffset;
    }
    else{

        QImage icon(config.iconPath);

        if (!icon.isNull()){


            useIcon = true;

            if (config.text.isEmpty()){

                qreal imagew = 0.8*boundingRect().width();
                qreal imageh = 0.8*boundingRect().height();
                normalIcon = icon.scaled(imagew,imageh,Qt::KeepAspectRatio);
                pressedIcon = normalIcon.convertToFormat(QImage::Format_Alpha8);

                // Center the image in the button.
                xIcon = (boundingBox.width() - icon.width())/2.0;
                yIcon = (boundingBox.height() - icon.height())/2.0;
            }
            else{

                switch (config.iconPosition){
                case ICON_ABOVE:
                    configForIconAbove(icon);
                    break;
                case ICON_BELOW:
                    configForIconBelow(icon);
                    break;
                case ICON_LEFT:
                    configForIconLeft(icon);
                    break;
                case ICON_RIGHT:
                    configForIconRight(icon);
                    break;
                }
                pressedIcon = normalIcon.convertToFormat(QImage::Format_Alpha8);

            }
        }
        else{
            // Same as emtpy icon
            qint32 textFlags = Qt::AlignCenter;
            QFontMetrics metrics(config.font);
            QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                          textFlags,config.text);
            yText = textBoundingBox.top() + textBoundingBox.height() + config.yOffset;
            xText = textBoundingBox.left() + config.xOffset;
        }
    }

    update();
}

void ColoUiButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    Q_UNUSED(widget)
    Q_UNUSED(option)

    // Painter set up to draw the shape
    QPen pen;
    QBrush brush;
    pen.setWidth(config.borderWidth);
    pen.setColor(config.borderColor);

    if (isPressed){
        painter->setBrush(QBrush(config.alternativeBackgroundColor));
    }
    else{
        painter->setBrush(QBrush(config.backgroundColor));
    }

    painter->setPen(pen);

    switch (config.shape){
    case SHAPE_ELLIPSE:
        painter->drawEllipse(boundingBox);
        break;
    case SHAPE_ROUNDED_RECT:
        painter->drawRoundedRect(boundingBox,config.roundeRectRadious,config.roundeRectRadious);
        break;
    default:
        painter->drawRect(boundingBox);
        break;
    }


    if (useIcon){
        if (isPressed){
            painter->drawImage(QPointF(xIcon,yIcon),pressedIcon);
        }
        else{
            painter->drawImage(QPointF(xIcon,yIcon),normalIcon);
        }
    }

    if (isPressed){
        pen.setColor(config.alternativeTextColor);
        pen.setWidth(0);
        brush.setColor(config.alternativeTextColor);
        painter->setBrush(QBrush(config.alternativeTextColor));
    }
    else{
        pen.setColor(config.textColor);
        pen.setWidth(0);
        brush.setColor(config.textColor);
        painter->setBrush(QBrush(config.textColor));
    }
    painter->setPen(pen);
    painter->setFont(config.font);
    painter->drawText(QPointF(xText,yText),config.text);

}

//---------------- CONFIGURE VARIABLES FOR DIFFERENT ICON POSITIONS -------------------------------------

void ColoUiButton::configForIconAbove(QImage icon){
    qint32 textFlags = Qt::AlignHCenter|Qt::AlignBottom;
    QFontMetrics metrics(config.font);
    QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                  textFlags,config.text);

    qreal tempY = textBoundingBox.top() -0.05*boundingBox.height() + config.yOffset;
    yText = tempY + textBoundingBox.height();
    xText = textBoundingBox.left() + config.xOffset;
    normalIcon = icon.scaledToHeight(0.9*tempY);

    // Checking it fits
    if (normalIcon.width() > boundingBox.width()){
        normalIcon = icon.scaledToWidth(boundingBox.width()*0.95);
    }

    yIcon = (tempY - normalIcon.height())/2.0;
    xIcon = (boundingBox.width() - normalIcon.width())/2.0;
}

void ColoUiButton::configForIconBelow(QImage icon){
    qint32 textFlags = Qt::AlignHCenter|Qt::AlignTop;
    QFontMetrics metrics(config.font);
    QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                  textFlags,config.text);

    qreal tempY = textBoundingBox.top() + 0.05*boundingBox.height() + config.yOffset;
    yText = tempY + textBoundingBox.height();
    xText = textBoundingBox.left() + config.xOffset;
    normalIcon = icon.scaledToHeight(0.9*(boundingBox.height() - yText));

    // Checking it fits
    if (normalIcon.width() > boundingBox.width()){
        normalIcon = icon.scaledToWidth(boundingBox.width()*0.95);
    }

    xIcon = (boundingBox.width() - normalIcon.width())/2.0;
    yIcon = yText + (boundingBox.height() - yText - normalIcon.height())/2.0;
}

void ColoUiButton::configForIconLeft(QImage icon){
    qint32 textFlags = Qt::AlignVCenter|Qt::AlignRight;
    QFontMetrics metrics(config.font);
    QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                  textFlags,config.text);

    yText = textBoundingBox.top() + textBoundingBox.height() + config.yOffset;
    xText = textBoundingBox.left()-0.05*boundingBox.width() + config.xOffset;
    normalIcon = icon.scaledToWidth(xText*0.9);

    // Checking it fits
    if (normalIcon.height() > boundingBox.height()){
        normalIcon = icon.scaledToHeight(boundingBox.height()*0.95);
    }

    xIcon = (xText - normalIcon.width())/2;
    yIcon = (boundingBox.height() - normalIcon.height())/2.0;
}


void ColoUiButton::configForIconRight(QImage icon){
    qint32 textFlags = Qt::AlignVCenter|Qt::AlignLeft;
    QFontMetrics metrics(config.font);
    QRectF textBoundingBox = metrics.boundingRect(0,0,boundingBox.width(),boundingBox.height(),
                                                  textFlags,config.text);

    yText = textBoundingBox.top() + textBoundingBox.height() + config.yOffset;
    xText = textBoundingBox.left()+0.05*boundingBox.width() + config.xOffset;
    normalIcon = icon.scaledToWidth((boundingBox.width() -xText)*0.9);

    // Checking if the icon fits
    if (normalIcon.height() > boundingBox.height()){
        normalIcon = icon.scaledToHeight(boundingBox.height()*0.95);
    }

    xIcon = xText + textBoundingBox.width() + (boundingBox.width() - xText - textBoundingBox.width() - normalIcon.width())/2.0;
    yIcon = (boundingBox.height() - normalIcon.height())/2.0;
}


//------------------------------ MOUSE EVENTS -----------------------------------------------------------

void ColoUiButton::mousePressEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    isPressed = true;
    update();
}

void ColoUiButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
    isPressed = false;
    update();
    if (signalSender != NULL){
        signalSender->sendSignal(signalInfo);
    }
}


