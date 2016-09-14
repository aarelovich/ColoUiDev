#include "colouitext.h"

//ColoUiTextInputDialog ColoUiText::inputDialog;

ColoUiText::ColoUiText(QString name, ColoUiTextInputDialog *diag, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{
    config.height = 400;
    config.width = 800;
    config.text = "Example Text";
    config.backgroundColor = QColor(Qt::gray);
    boundingBox = QRectF(0,0,config.width,config.height);
    inputDialog = diag;
}

void ColoUiText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget)
    Q_UNUSED(option)

    painter->drawRect(0,0,config.width,config.height);
    painter->drawText(boundingBox,config.text);

}

void ColoUiText::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if (inputDialog != NULL){
        inputDialog->setInput(config.text);
        if (inputDialog->exec()){
            config.text = inputDialog->getInput();
            update();
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void ColoUiText::setConfiguration(ColoUiElementConfig c){
    config = c;
    update();
}
