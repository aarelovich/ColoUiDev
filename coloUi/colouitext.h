#ifndef COLOUITEXT_H
#define COLOUITEXT_H

#include "colouielement.h"
#include "colouitextinputdialog.h"
#include <QTextDocument>
#include <QSize>
#include <QAbstractTextDocumentLayout>

class TextAnalyzer {

public:
    TextAnalyzer();

    void setFont(QFont f);
    QString getText() const;
    void setText(QString t);
    void appendText(QString newText);
    void newLine();
    void backSpace();
    QPointF currentCursorPos();
    qreal getCharHeight() const {return charHeight;}

private:
    QStringList textLines;
    qint32 colPos;
    qreal xCursor;
    qint32 linePos;
    qreal yCursor;
    qreal charHeight;
    qreal charWidth;
    QFont font;
    QFontMetricsF *fm;
};


class ColoUiText : public ColoUiElement
{
public:
    ColoUiText(QString name, ColoUiTextInputDialog *diag, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);

    QString getText() const {return config.getString(CPR_TEXT);}

    void clearText();

    void setText(QString text);

    void appendText(QString text);

    void appendFormattedText(QString text,
                             QFont font,
                             QColor textcolor);

    void appendFormattedText(QString text,
                             QFont font,
                             QColor textcolor,
                             QColor background);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void wheelEvent(QGraphicsSceneWheelEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void keyPressEvent(QKeyEvent *e);

private:
    ColoUiTextInputDialog *inputDialog;
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
    bool movingText;
    bool scrollEnabled;
    QRegExp acceptedInput;

    TextAnalyzer textAnalyzer;
    bool editingEnabled;
    QPointF cursor;

    void updateTextBoundingBox();

};

#endif // COLOUITEXT_H
