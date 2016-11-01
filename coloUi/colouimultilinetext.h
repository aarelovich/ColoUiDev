#ifndef COLOUITEXT_H
#define COLOUITEXT_H

#include "colouielement.h"
#include <QTextDocument>
#include <QApplication>
#include <QClipboard>
#include <QSize>
#include <QTime>
#include <QAbstractTextDocumentLayout>

class ColoUiMultiLineText : public ColoUiElement
{
public:
    ColoUiMultiLineText(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    QString getText() const {return config.getString(CPR_TEXT);}

    void clearText();

    void setText(QString text);



    void appendText(QString text);

    void appendFormattedText(QString text,
                             QFont font,
                             QColor textcolor,
                             bool appendNewLine = true);

    void appendFormattedText(QString text,
                             QFont font,
                             QColor textcolor,
                             bool appendNewLine,
                             QColor background);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    void wheelEvent(QGraphicsSceneWheelEvent *e);

private:
    // Show text variables
    QRectF textBoundingBox;
    qreal yDisplacement;
    qreal yLastScrollPoint;

    // Scrollbar variables
    qreal scrollBarWidth;
    qreal endScrollBarPoint;
    qreal sliderHeight;
    qreal sliderPosition;
    qreal scrollBarX;
    qreal deltaY;
    bool movingSlider;
    bool scrollEnabled;

    void updateTextBoundingBox();

    QString processTextForLineBreaks(QString input);

};

#endif // COLOUITEXT_H
