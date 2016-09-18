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
#include <QPen>
#include <QFontMetrics>

typedef enum {ST_MOUSE_CLICK, ST_REQ_INPUT_DIAG} ColoUiSignalEventType;

typedef enum {CUI_BUTTON, CUI_TEXT, CUI_LIST} ColoUiElementType;

typedef enum {ABSOLUTE, PORCENTUAL} ColoUiDimSpec;

typedef enum {SHAPE_ELLIPSE, SHAPE_RECT, SHAPE_ROUNDED_RECT} ColoUiShape;

typedef enum {ICON_ABOVE,ICON_RIGHT,ICON_LEFT,ICON_BELOW} ColoUiIconPosition;

typedef enum {TRANSITION_UP, TRANSITION_DOWN, TRANSITION_RIGHT, TRANSITION_LEFT} ColoUiTransitionType;

typedef struct {

    ColoUiSignalEventType type;
    QString elementID;
    QVariant data;

} ColoUiSignalEventInfo;

typedef struct {
   QString viewA;
   QString viewB;
   qint32 transitionLengthInMS;
   qint32 transitionSteps;
   ColoUiTransitionType type;
} ColoUiTransition;

typedef struct {
    quint16 x;
    quint16 y;
    quint16 width;
    quint16 height;
    QColor backgroundColor;    
    QColor alternativeBackgroundColor;
    QColor textColor;
    QColor alternativeTextColor;
    QColor borderColor;
    qreal  borderWidth;
    QString text;
    ColoUiShape shape;
    ColoUiIconPosition iconPosition;
    QFont font;
    qint32 fontSize;
    qreal roundeRectRadious;
    QString iconPath;
    qreal yOffset;
    qreal xOffset;
} ColoUiElementConfig;

//--------------------- CONSTANTS ------------------

#define RESIZE_EVENT_TIMER_MS  100

//--------------------- ERRORS ---------------------
#define ERROR_NAME_IN_USE                         "NAME_IN_USE"
#define ERROR_ELEMENT_OVERLAPS                    "ELEMENT_OVERLAPS"
#define ERROR_ELEMENT_NOT_CONTAINED_IN_VIEW       "ELEMENT_NOT_CONTAINED_IN_VIEW"
#define ERROR_UNKNOWN_ELEMENT_TYPE                "UNKNOWN_ELEMENT_TYPE"
#define ERROR_VIEW_NAME_IN_USE                    "VIEW_NAME_IN_USE"
#define ERROR_VIEW_OVERLAPS                       "VIEW_OVERLAPS"
#define ERROR_VIEW_NOT_CONTAINED_IN_DRAWING_AREA  "ELEMENT_NOT_CONTAINED_IN_VIEW"

namespace ColoUi {

inline static ColoUiElementConfig standardElementConfig(){

    ColoUiElementConfig c;
    c.alternativeBackgroundColor = QColor(Qt::gray).lighter();
    c.alternativeTextColor = QColor(Qt::black).lighter();
    c.backgroundColor = QColor(Qt::gray);
    c.borderColor = QColor(Qt::black);
    c.borderWidth = 0;
    c.font = QFont("FreeSans");
    c.fontSize = 10;
    c.iconPath = "";
    c.iconPosition = ICON_LEFT;
    c.roundeRectRadious = 10;
    c.shape = SHAPE_RECT;
    c.text = "ColoUi";
    c.textColor = QColor(Qt::black);
    c.width = 100;
    c.height = 50;
    c.x = 0;
    c.y = 0;
    c.yOffset = 0;
    c.xOffset = 0;
    return c;
}

inline static ColoUiTransition standardTransition() {
    ColoUiTransition t;
    t.transitionLengthInMS = 2000;
    t.transitionSteps = 100;
    t.type = TRANSITION_RIGHT;
    t.viewA = "";
    t.viewB = "";
    return t;
}

}



#endif // COLOUIDEFINES_H
