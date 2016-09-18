#ifndef COLOUICONTAINER_H
#define COLOUICONTAINER_H

#include "colouiview.h"
#include "colouitransitionscreen.h"
#include <QGraphicsView>
#include <QResizeEvent>
#include <QTimer>


class ColoUiContainer : public QGraphicsView, public ColoUiBase
{
    Q_OBJECT
public:
    ColoUiContainer();

    void setDrawingArea(quint16 width, quint16 height);
    void setForceNoScrollBars(bool isTrue);
    QString createView(QString ID, quint16 x, quint16 y, quint16 w, quint16 h, bool dimensionsAreRelative = false);
    ColoUiView *getViewByID(QString id) const;
    ColoUiTextInputDialog *getInputDialog() const {return inputDialog;}
    ColoUiSignalManager *getSignalManager() const {return signalManager;}
    void addTransition(ColoUiTransition t);
    void startTranstion(QString viewA, QString viewB);
    void drawUi();

public slots:
    void on_resizeEventDone();
    void on_transitionTimerTimeout();

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);

private:

    // The list of views in the container.
    QHash<QString,ColoUiView*> views;
    QHash<QString,bool> drawnViews;
    QVector<ColoUiTransition> transitions;

    // To know when a resizing event has ended
    QTimer resizeEventTimer;

    // The input text dialog for Text Elements
    ColoUiTextInputDialog *inputDialog;

    // For element connection to the outside
    ColoUiSignalManager *signalManager;

    // The transition screen used for animating views
    ColoUiTransitionScreen *transitionScreen;
    qreal activeTransitionDelta;
    bool activeTransitionInX;
    qint32 activeTransitionCounter;
    QTimer transitionTimer;
    QString viewToRemove;
    QString viewToInsert;

    // Scene fitting and resizing
    bool forceNoScrollBars;
    void resizeSceneRect();
    bool uiHasBeenDrawn;

};

#endif // COLOUICONTAINER_H
