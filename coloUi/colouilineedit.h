#ifndef COLOUILINEEDIT_H
#define COLOUILINEEDIT_H

#include "colouielement.h"
#include "colouikeyboard.h"
#include <QTextDocument>
#include <QApplication>
#include <QClipboard>
#include <QAbstractTextDocumentLayout>


class ColoUiLineEdit : public ColoUiElement, public KeyboardInterface
{
public:
    ColoUiLineEdit(QString name, ColoUiSignalManager * ss = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setConfiguration(ColoUiConfiguration c);
    void softKeyboardInterface(ColoUiKeyType kt, QString data);

    QString getText() const {return config.getString(CPR_TEXT);}
    void setText(QString text);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void focusOutEvent(QFocusEvent *e);

private:
    bool editingEnabled;
    qint32 colCursor;
    qint32 colStart;
    qint32 colEnd;
    qreal effectiveWidth;
    QFontMetricsF *fm;
    qreal yoffset;
    QRegExp acceptedInput;

    void correctLineColWindow(QString text);
};

#endif // COLOUILINEEDIT_H
