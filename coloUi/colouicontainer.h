#ifndef COLOUICONTAINER_H
#define COLOUICONTAINER_H

#include "colouisignalmanager.h"
#include "colouibutton.h"
#include "colouilist.h"
#include "colouitextinputdialog.h"
#include "colouitext.h"
#include <QGraphicsView>
#include <QResizeEvent>
#include <QTimer>


class ColoUiContainer : public QGraphicsView, public ColoUiBase
{
    Q_OBJECT
public:
    ColoUiContainer();

    void setDrawingArea(qreal width, qreal height);
    void setDrawDrawAreaRect(bool enable);

public slots:
    void on_coloUiSignal();
    void on_resizeEventDone();

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);

private:
    QTimer resizeEventTimer;
    ColoUiSignalManager *signalManager;
    ColoUiTextInputDialog *inputDialog;
    QGraphicsRectItem *drawAreaRect;
    void resizeSceneRect();
    bool drawDrawAreaRect;
    qreal currentScale;

};

#endif // COLOUICONTAINER_H
