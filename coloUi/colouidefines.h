#ifndef COLOUIDEFINES_H
#define COLOUIDEFINES_H

#include <QGraphicsItem>
#include <QString>
#include <QVariant>
#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QHash>

typedef enum {ST_MOUSE_CLICK, ST_REQ_INPUT_DIAG} ColoUiSignalEventType;

typedef enum {CUI_BUTTON, CUI_TEXT, CUI_LIST} ColoUiElementType;

typedef enum {RP_ABSOLUTE, RP_LEFT, RP_BELOW, RP_ABOVE, RP_RIGHT} ColoUiRelativePostion;

typedef enum {ES_ABSOLUTE, ES_FIT_IN_WIDTH, ES_FIT_IN_HEIGHT} ColoUiElementSizing;

typedef struct {

    ColoUiSignalEventType type;
    QString elementID;
    QVariant data;

} ColoUiSignalEventInfo;

typedef struct {
    qreal x;
    qreal y;
    qreal width;
    qreal height;
    QColor backgroundColor;    
    QColor textColor;
    QColor borderColor;
    qreal  borderWidth;
    QString text;

} ColoUiElementConfig;



#define RESIZE_EVENT_TIMER_MS  100


#endif // COLOUIDEFINES_H
