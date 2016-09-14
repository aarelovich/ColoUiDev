#include "colouilist.h"

ColoUiList::ColoUiList(QString name, ColoUiSignalManager *ss):ColoUiElement(name,ss)
{

    // Default values
    config.backgroundColor = QColor(Qt::gray);
    config.textColor = QColor(Qt::black);
    config.height = 3000;
    config.width = 200;
    boundingBox = QRectF(0,0,config.width,config.height);
    setConfiguration(config);

    signalInfo.type = ST_MOUSE_CLICK;
}

void ColoUiList::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget)
    Q_UNUSED(option)

    qreal itemHeight = config.height/10;
    qint32 N = qCeil(config.height/itemHeight);
    bool on = false;
    qreal y = yStartPoint;
    for (qint32 i = 0; i < N; i++){
        if (on){
            painter->setBrush(QBrush(Qt::red));
        }
        else{
            painter->setBrush(QBrush(Qt::blue));
        }
        painter->drawRect(QRectF(0,y,config.width,itemHeight));
        y = y + itemHeight;
        on = !on;
    }

}

void ColoUiList::mousePressEvent(QGraphicsSceneMouseEvent *e){
    QPointF now = mapToScene(e->pos());
    lastYpoint = now.y();
}

void ColoUiList::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
    Q_UNUSED(e)
}

void ColoUiList::mouseMoveEvent(QGraphicsSceneMouseEvent *e){

    QPointF now = mapToScene(e->pos());
    qreal dy = lastYpoint - now.y();
    lastYpoint = now.y();
    yStartPoint = yStartPoint - dy;
    yStartPoint = qMin(qMax(yStartPoint,maxYDisplacement),0.0);
    //qDebug() << "Max Y Displacement" << maxYDisplacement << "dy" << dy << yStartPoint;
    update();

}

void ColoUiList::setConfiguration(ColoUiElementConfig c){
    config = c;    
    maxYDisplacement = SCREEN_HEIGHT - config.height;
    maxYDisplacement = qMin(0.0,maxYDisplacement);
    //qDebug() << "Max Y Displacement" << maxYDisplacement;
    update();
}
