#ifndef COLOUIKEYBOARD_H
#define COLOUIKEYBOARD_H

#include "colouidefines.h"
#include "colouimultilinetext.h"
#include <QFontDatabase>
#include <QPainterPath>
#include <QIcon>

typedef enum {KS_NORMAL,KS_SHIFT,KS_ALT} KeyState;

class Key {

public:
    Key(ColoUiKeyType kt = KT_TEXT, ColoUiKeyType altKt = KT_NOTYPE);
    void setDimension(qint32 x, qint32 y, qint32 w, qint32 h, qreal ux, qreal uy);
    void setStringsForStates(QString normal_str, QString shift_str, QString alt_str){
        normal = normal_str;
        shift = shift_str;
        alternate = alt_str;
    }   
    void setToggle(bool enable) {isToggle = enable;}
    void switchToState(KeyState nstate);
    void drawKey(QPainter *painter) const;
    bool inKey(QPointF p) const { return key.contains(p); }
    void press();
    void release();

    ColoUiKeyType getKeyType() const;

    KeyState getKeyState() const {return keyState;}
    QString getKeyContent() const;

private:
    QString normal;
    QString shift;
    QString alternate;

    bool isToggle;
    bool isPressed;

    KeyState keyState;
    ColoUiKeyType keyType;
    ColoUiKeyType alternateKeyType;

    // The key bounding box
    QRectF key;


    QPainterPath icon;
    QPainterPath iconAlt;
    bool useIcon;
    bool useIconAlt;

    static const QColor background;
    static const QColor altbackground;
    static const QColor textAndBorderColor;
    static const QColor altTextColor;

    static QPainterPath backspacePath(qreal w, qreal h);
    static QPainterPath returnPath(qreal w, qreal h);
    static QPainterPath shiftPath(qreal w, qreal h);
    static QPainterPath altPath(qreal w, qreal h);
    static QPainterPath copyPath(qreal w, qreal h);
    static QPainterPath pastePath(qreal w, qreal h);
    static QPainterPath hidePath(qreal w, qreal h);
    static QPainterPath upPath(qreal w, qreal h);
    static QPainterPath downPath(qreal w, qreal h);
    static QPainterPath rightPath(qreal w, qreal h);
    static QPainterPath leftPath(qreal w, qreal h);

};

//-------------------- Keyboard Class --------------------------

class ColoUiKeyboard : public QGraphicsItem
{
public:
    ColoUiKeyboard(qreal w, qreal he);

    void setTextElement(ColoUiMultiLineText *view){textView = view;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const {return boundingBox;}

    // ColoUi Font!! (Unispace from the web)
    static void loadFont();
    static QFont getFont() {return font;}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    QRectF boundingBox;
    QVector< QVector<Key> > keyboard;
    ColoUiMultiLineText *textView;
    qint32 rowPressed;
    qint32 keyPressed;
    qreal keyboardRowHeight;
    static QFont font;

    void updateRowAndKeyPressed(QPointF p);
    void switchKeyboardToState(KeyState st);
};

#endif // COLOUIKEYBOARD_H
