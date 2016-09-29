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
#include <QLinearGradient>
#include <QRadialGradient>

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< COLO UI PROPERTIES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#define  CPR_X                                   "x"
#define  CPR_Y                                   "y"
#define  CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST      "numberOfItemsToViewInList"
#define  CPR_WIDTH                               "width"
#define  CPR_HEIGHT                              "height"
#define  CPR_BORDER_WIDTH                        "borderWidth"
#define  CPR_TEXT                                "text"
#define  CPR_SHAPE                               "shape"
#define  CPR_ICON_POSITION                       "iconPosition"
#define  CPR_FONT                                "font"
#define  CPR_ROUNDED_RECT_RADIOUS                "roundedRectRadious"
#define  CPR_ICON_PATH                           "iconPath"
#define  CPR_Y_OFFSET                            "yOffset"
#define  CPR_X_OFFSET                            "xOffset"
#define  CPR_ALTERNATIVE_BACKGROUND_ON_HOVER     "alternativeBackgroundOnHover"
#define  CPR_READ_ONLY                           "readOnly"
#define  CPR_NAME                                "name"
#define  CPR_BACKGROUND_COLOR                    "backgroundColor"
#define  CPR_ALTERNATIVE_BACKGROUND_COLOR        "alternativeBackgroundColor"
#define  CPR_BORDER_COLOR                        "borderColor"
#define  CPR_TEXT_COLOR                          "textColor"
#define  CPR_ALTERNATIVE_TEXT_COLOR              "alternativeTextColor"
#define  CPR_USE_CONFIGURATION                   "useConfig"
#define  CPR_VALUES_RELATIVE                     "relativeValues"
#define  CPR_LIST_HEADER_VISIBLE                 "listHeaderVisible"

#define  CPR_TRANSITION_VIEW_A                   "viewA"
#define  CPR_TRANSITION_VIEW_B                   "viewB"
#define  CPR_TRANSITION_TIME                     "duration"
#define  CPR_TRANSITION_STEPS                    "steps"
#define  CPR_TRANSITION_TYPE                     "type"

static const QStringList ColoUiProperties = QStringList() << CPR_ALTERNATIVE_BACKGROUND_ON_HOVER
                                                          << CPR_NAME
                                                          << CPR_BORDER_COLOR
                                                          << CPR_FONT
                                                          << CPR_HEIGHT
                                                          << CPR_ICON_PATH
                                                          << CPR_ICON_POSITION
                                                          << CPR_NUMBER_OF_ITEM_TO_VIEW_IN_LIST
                                                          << CPR_READ_ONLY
                                                          << CPR_ROUNDED_RECT_RADIOUS
                                                          << CPR_USE_CONFIGURATION
                                                          << CPR_LIST_HEADER_VISIBLE
                                                          << CPR_SHAPE
                                                          << CPR_TEXT
                                                          << CPR_WIDTH
                                                          << CPR_X
                                                          << CPR_X_OFFSET
                                                          << CPR_Y
                                                          << CPR_Y_OFFSET
                                                          << CPR_BORDER_WIDTH
                                                          << CPR_ALTERNATIVE_BACKGROUND_COLOR
                                                          << CPR_ALTERNATIVE_TEXT_COLOR
                                                          << CPR_BACKGROUND_COLOR
                                                          << CPR_BORDER_COLOR
                                                          << CPR_TEXT_COLOR
                                                          << CPR_TRANSITION_VIEW_A
                                                          << CPR_TRANSITION_VIEW_B
                                                          << CPR_TRANSITION_STEPS
                                                          << CPR_TRANSITION_TYPE
                                                          << CPR_TRANSITION_TIME
                                                          << CPR_VALUES_RELATIVE;

#define CUI_LANG_DRAW_AREA      "DRAW_AREA"
#define CUI_LANG_BUTTON         "BUTTON"
#define CUI_LANG_LIST           "LIST"
#define CUI_LANG_VIEW           "VIEW"
#define CUI_LANG_GRADIENT       "GRADIENT"
#define CUI_LANG_COLOR          "COLOR"
#define CUI_LANG_CONFIG         "CONFIG"
#define CUI_LANG_ITEM           "ITEM"
#define CUI_LANG_TRANSITION     "TRANSITION"
#define CUI_LANG_HEADER         "HEADER"
#define CUI_LANG_DONE           "DONE"
#define CUI_LANG_INCLUDE        "INCLUDE"

static const QStringList ColoUiDeclarations = QStringList() << CUI_LANG_DRAW_AREA
                                                            << CUI_LANG_VIEW
                                                            << CUI_LANG_GRADIENT
                                                            << CUI_LANG_COLOR
                                                            << CUI_LANG_LIST
                                                            << CUI_LANG_CONFIG
                                                            << CUI_LANG_TRANSITION
                                                            << CUI_LANG_DONE
                                                            << CUI_LANG_BUTTON
                                                            << CUI_LANG_HEADER
                                                            << CUI_LANG_ITEM
                                                            << CUI_LANG_INCLUDE;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< COLO UI PARAMETER VALUES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#define  CPA_ELLIPSE              0
#define  CPA_RECT                 1
#define  CPA_ROUND_RECT           2

#define  CPA_DOWN                 3
#define  CPA_LEFT                 4
#define  CPA_RIGHT                5
#define  CPA_UP                   6

#define  CPA_GRAD_RADIAL          7
#define  CPA_GRAD_DIAGONAL        8
#define  CPA_GRAD_HORIZONTAL      9
#define  CPA_GRAD_VERTICAL        10
#define  CPA_GRAD_NONE            11

#define  CPA_BOLD                 12
#define  CPA_ITALIC               13

static const QVector<qint32> ColoUiDirection = QVector<qint32>() << CPA_DOWN << CPA_LEFT << CPA_RIGHT << CPA_UP;
static const QVector<qint32> ColoUiShape     = QVector<qint32>() << CPA_ELLIPSE << CPA_ROUND_RECT << CPA_RECT;
static const QVector<qint32> ColoUiGradType  = QVector<qint32>() << CPA_GRAD_RADIAL << CPA_GRAD_DIAGONAL
                                                                 << CPA_GRAD_HORIZONTAL << CPA_GRAD_VERTICAL << CPA_GRAD_NONE;
static const QVector<qint32> ColoUiFontMods  = QVector<qint32>() << CPA_BOLD << CPA_ITALIC;


static QHash<QString,qint32> initParameterMap(){
    QHash<QString,qint32>  m;
    m["Ellipse"]        = CPA_ELLIPSE;
    m["Rect"]           = CPA_RECT;
    m["RoundRect"]      = CPA_ROUND_RECT;
    m["Down"]           = CPA_DOWN;
    m["Up"]             = CPA_UP;
    m["Right"]          = CPA_RIGHT;
    m["Left"]           = CPA_LEFT;
    m["GradDiagonal"]   = CPA_GRAD_DIAGONAL;
    m["GradHorizontal"] = CPA_GRAD_HORIZONTAL;
    m["GradVertical"]   = CPA_GRAD_VERTICAL;
    m["GradRadial"]     = CPA_GRAD_RADIAL;
    m["GradNone"]       = CPA_GRAD_NONE;
    m["Bold"]           = CPA_BOLD;
    m["Italic"]         = CPA_ITALIC;
    return m;
}
static const QHash<QString,qint32> ColoUiParameters = initParameterMap();

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< COLO UI INTERNAL REFERENCES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define INTERNAL_COLOR_LIST              "colorList"
#define INTERNAL_GRAD_TYPE               "gradType"
#define INTERNAL_FONT_FAMILY             "fontFamily"
#define INTERNAL_FONT_SIZE               "fontSize"
#define INTERNAL_FONT_ITALIC             "fontItalic"
#define INTERNAL_FONT_BOLD               "fontBold"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< COLO UI ENUMS & TYPEDEFS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

typedef enum {ST_MOUSE_CLICK, ST_MOUSE_DOUBLE_CLICK, ST_MOUSE_RIGHT_CLICK} ColoUiSignalEventType;

typedef enum {CUI_BUTTON, CUI_TEXT, CUI_LIST} ColoUiElementType;

typedef struct {
    ColoUiSignalEventType type;
    QString elementID;
    QVariant data;
} ColoUiSignalEventInfo;


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#define RESIZE_EVENT_TIMER_MS  100

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ERROR STRINGS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define ERROR_NAME_IN_USE                         "NAME_IN_USE"
#define ERROR_ELEMENT_OVERLAPS                    "ELEMENT_OVERLAPS"
#define ERROR_ELEMENT_NOT_CONTAINED_IN_VIEW       "ELEMENT_NOT_CONTAINED_IN_VIEW"
#define ERROR_UNKNOWN_ELEMENT_TYPE                "UNKNOWN_ELEMENT_TYPE"
#define ERROR_VIEW_NAME_IN_USE                    "VIEW_NAME_IN_USE"
#define ERROR_VIEW_OVERLAPS                       "VIEW_OVERLAPS"
#define ERROR_VIEW_NOT_CONTAINED_IN_DRAWING_AREA  "ELEMENT_NOT_CONTAINED_IN_VIEW"
#define ERROR_NAMES_CANNOT_CONTAIN_DOT            "NAMES_CANNOT_CONTAIN_DOT"
#define ERROR_VIEWA_NOT_FOUND                     "VIEWA_NOT_FOUND"
#define ERROR_VIEWB_NOT_FOUND                     "VIEWB_NOT_FOUND"
#define ERROR_VIEW_SIZES_ARE_DIFFERENT            "VIEW_SIZES_ARE_DIFFERENT"
#define ERROR_TRANSITION_EXISTS                   "TRANSITION_EXISTS"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< COLO UI ENUMS & TYPEDEFS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//namespace ColoUi {

//inline static ColoUiConfiguration standardElementConfig(){

//    ColoUiConfiguration c;
//    c.alternativeBackgroundColor = QColor(Qt::gray).lighter();
//    c.alternativeTextColor = QColor(Qt::gray);
//    c.backgroundColor = QColor(Qt::gray);
//    c.borderColor = QColor(Qt::black);
//    c.borderWidth = 0;
//    c.font = QFont("FreeSans");
//    c.fontSize = 10;
//    c.iconPath = "";
//    c.iconPosition = ICON_LEFT;
//    c.roundeRectRadious = 10;
//    c.shape = SHAPE_RECT;
//    c.text = "ColoUi";
//    c.textColor = QColor(Qt::black);
//    c.width = 100;
//    c.height = 50;
//    c.x = 0;
//    c.y = 0;
//    c.yOffset = 0;
//    c.xOffset = 0;
//    c.alternativeBackgroundOnHover = false;
//    c.readOnly = true;
//    c.numberOfItemsToViewInList = 5;
//    return c;
//}

//inline static ColoUiTransition standardTransition() {
//    ColoUiTransition t;
//    t.transitionLengthInMS = 2000;
//    t.transitionSteps = 100;
//    t.type = TRANSITION_RIGHT;
//    t.viewA = "";
//    t.viewB = "";
//    return t;
//}

//}



#endif // COLOUIDEFINES_H
