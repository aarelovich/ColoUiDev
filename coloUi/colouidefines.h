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
#include <QFontMetrics>

typedef enum {ST_MOUSE_CLICK, ST_REQ_INPUT_DIAG} ColoUiSignalEventType;

typedef enum {CUI_BUTTON, CUI_TEXT, CUI_LIST} ColoUiElementType;

typedef enum {ABSOLUTE, PORCENTUAL} ColoUiDimSpec;

typedef enum {SHAPE_ELLIPSE, SHAPE_RECT, SHAPE_ROUNDED_RECT} ColoUiShape;

typedef struct {

    ColoUiSignalEventType type;
    QString elementID;
    QVariant data;

} ColoUiSignalEventInfo;

typedef struct {
    quint16 x;
    quint16 y;
    quint16 width;
    quint16 height;
    QColor backgroundColor;    
    QColor textColor;
    QColor borderColor;
    qreal  borderWidth;
    QString text;
    ColoUiShape shape;
    QFont font;

} ColoUiElementConfig;



#define RESIZE_EVENT_TIMER_MS  100

//--------------------- ERRORS ---------------------
#define ERROR_NAME_IN_USE                         "NAME_IN_USE"
#define ERROR_ELEMENT_OVERLAPS                    "ELEMENT_OVERLAPS"
#define ERROR_ELEMENT_NOT_CONTAINED_IN_VIEW       "ELEMENT_NOT_CONTAINED_IN_VIEW"
#define ERROR_UNKNOWN_ELEMENT_TYPE                "UNKNOWN_ELEMENT_TYPE"
#define ERROR_VIEW_NAME_IN_USE                    "VIEW_NAME_IN_USE"
#define ERROR_VIEW_OVERLAPS                       "VIEW_OVERLAPS"
#define ERROR_VIEW_NOT_CONTAINED_IN_DRAWING_AREA  "ELEMENT_NOT_CONTAINED_IN_VIEW"



#endif // COLOUIDEFINES_H
