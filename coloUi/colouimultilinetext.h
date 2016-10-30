#ifndef COLOUITEXT_H
#define COLOUITEXT_H

#include "colouielement.h"
#include "colouikeyboard.h"
#include <QTextDocument>
#include <QApplication>
#include <QClipboard>
#include <QSize>
#include <QTime>
#include <QAbstractTextDocumentLayout>

class TextManager {

public:
    TextManager();

    void configureAnalyzer(QFont f, qreal parentW, qreal parentH);
    QString getText() const;
    void setText(QString t);
    void copyText(const QString &t);
    void appendText(QString newText);
    qreal newLine(qreal yOffset);
    void backSpace();
    qreal arrowMoveCursor(qint32 direction, qreal yoffset);
    QPointF setCursorPosition(QPointF click, qreal yoffset);
    QPointF currentCursorPos();
    qreal getCharHeight() const {return charHeight;}
    qreal getMaxCharWidth() const { return fm->maxWidth(); }
    void goToLinePos(bool start);

private:
    QStringList textLines;
    qint32 numberOfVisibleLines;
    qint32 colPos;
    qreal xCursor;
    qint32 linePos;
    qreal yCursor;
    qreal charHeight;
    qreal maxLineWidth;
    qreal maxHeight;
    QFont font;
    QFontMetricsF *fm;

};

class ColoUiMultiLineText : public ColoUiElement, public KeyboardInterface
{
public:
    ColoUiMultiLineText(QString name, ColoUiSignalManager * ss = 0);

    // Virtual Functions
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);
    void softKeyboardInterface(ColoUiKeyType kt, QString data);

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
    void focusOutEvent(QFocusEvent *e);

    void keyPressEvent(QKeyEvent *e);

private:
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

    TextManager textManager;
    bool editingEnabled;
    QPointF cursor;

    void updateTextBoundingBox();

};

#endif // COLOUITEXT_H
