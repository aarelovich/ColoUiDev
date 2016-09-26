#ifndef COLOUICONTAINER_H
#define COLOUICONTAINER_H

#include "colouiview.h"
#include "colouitransitionscreen.h"
#include <QGraphicsView>
#include <QResizeEvent>
#include <QTimer>
#include <QElapsedTimer>


class ColoUiContainer : public QGraphicsView, public ColoUiBase
{
    Q_OBJECT
public:
    ColoUiContainer();

    // Defining the draw area and scaling method
    void setDrawingArea(quint16 width, quint16 height);
    void setTransitionScreenColor(QColor c)  {transitionScreen->setShowColor(c);}
    void setForceNoScrollBars(bool isTrue);

    // Managing views
    QString createView(QString ID, quint16 x, quint16 y, quint16 w, quint16 h, bool dimensionsAreRelative = false);
    ColoUiView *getViewByID(QString id) const;

    // Required by view creation
    ColoUiTextInputDialog *getInputDialog() const {return inputDialog;}

    // Getting the signal manager to make connections
    ColoUiSignalManager *getSignalManager() const {return signalManager;}

    // Transition control functions
    QString addTransition(ColoUiConfiguration t);
    void startTranstion(QString viewA, QString viewB);

    ColoUiElement *element(QString id) const;

    void drawUi();

    // Delete all elements
    void deleteUi();

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
    QVector<ColoUiConfiguration> transitions;

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
