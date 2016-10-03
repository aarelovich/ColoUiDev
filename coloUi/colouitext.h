#ifndef COLOUITEXT_H
#define COLOUITEXT_H

#include "colouielement.h"
#include "colouitextinputdialog.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

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
    void wheelEvent(QGraphicsSceneWheelEvent *e);

private:
    ColoUiTextInputDialog *inputDialog;
    QRectF textBoundingBox;
    qreal yDisplacement;
    qreal yLastScrollPoint;

    void updateTextBoundingBox();

};

#endif // COLOUITEXT_H
