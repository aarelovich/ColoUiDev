#ifndef COLOUICONTAINER_H
#define COLOUICONTAINER_H

#include "colouidrawinggrid.h"
#include "colouiview.h"
#include <QGraphicsView>
#include <QResizeEvent>
#include <QTimer>


class ColoUiContainer : public QGraphicsView, public ColoUiBase
{
    Q_OBJECT
public:
    ColoUiContainer();

    void setDrawingArea(quint16 width, quint16 height);
    void setDrawDrawAreaRect(bool enable);
    void setForceNoScrollBars(bool isTrue);
    QString createView(QString ID, quint16 x, quint16 y, quint16 w, quint16 h);
    ColoUiView *getViewByID(QString id) const;
    ColoUiTextInputDialog *getInputDialog() const {return inputDialog;}

public slots:
    void on_coloUiSignal();
    void on_resizeEventDone();

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);

private:

    // The list of views in the container.
    QHash<QString,ColoUiView*> views;

    // To know when a resizing event has ended
    QTimer resizeEventTimer;

    // The input text dialog for Text Elements
    ColoUiTextInputDialog *inputDialog;

    // The draw area
    ColoUiDrawingGrid *drawAreaRect;

    // Scene fitting and resizing
    bool drawDrawAreaRect;
    bool forceNoScrollBars;
    qreal currentScale;    
    void resizeSceneRect();


};

#endif // COLOUICONTAINER_H
