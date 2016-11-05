#ifndef COLOUICONTAINER_H
#define COLOUICONTAINER_H

#include "colouiview.h"
#include "colouitransitionscreen.h"
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QPair>


class ColoUiContainer : public QGraphicsView, public ColoUiBase
{
    Q_OBJECT
public:
    ColoUiContainer(QWidget *parent = 0);

    // Defining the draw area and scaling method
    void setDrawingArea(quint16 width, quint16 height);
    void setTransitionScreenColor(QColor c);

    // Managing views
    QString createView(QString ID, quint16 x, quint16 y, quint16 w, quint16 h, bool dimensionsAreRelative = false);
    ColoUiView *getViewByID(QString id) const;

    // Getting the signal manager to make connections
    ColoUiSignalEventInfo getSignalInfo() const { return signalManager->getSignalEventInfo();}

    // Transition control functions
    QString addTransition(ColoUiConfiguration t);
    void startTranstion(QString viewA, QString viewB);
    QVector<ColoUiConfiguration> getTransitions() const {return transitions;}

    // This is used to replace a placeholder element in place of a custom ColoUi like element.
    bool replacePlaceHolder(QString placeHolderID, ColoUiElement *customElement);

    // Get if a view is being shown
    bool isViewDrawn(QString viewID) const;

    // Access an element of the UI.
    ColoUiElement *getElement(QString id) const;

    // Get a list of all element handles.
    QStringList elementList() const;

    QString currentSizeStructure() const;

    // Actually draw the UI
    void drawUi();

    // Set and get configurations defined while creating
    void setGlobalConfigurations(QHash<QString,ColoUiConfiguration> configs) {parsedConfigs = configs;}
    ColoUiConfiguration getConfiguration(QString config) const;

    // Delete all elements
    void deleteUi();

public slots:
    void on_resizeEventDone();
    void on_transitionTimerTimeout();
    void on_keyboardTranstionTimerTimeout();

private slots:
    void on_coloUiSignal();

signals:
    void elementSignal();

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

    // The soft keyboard
    ColoUiKeyboard *softKeyboard;

    // For element connection to the outside
    ColoUiSignalManager *signalManager;

    // The resolution of the screen along the x coordinates
    QSizeF screenResolution;

    // Parsed configurations
    QHash<QString,ColoUiConfiguration> parsedConfigs;

    // The transition screen used for animating views
    ColoUiTransitionScreen *transitionScreen;
    qreal activeTransitionDelta;
    bool activeTransitionInX;
    qint32 activeTransitionCounter;
    QTimer transitionTimer;
    QString viewToRemove;
    QString viewToInsert;
    typedef QPair<QString,QString> StringPair;
    QVector<StringPair> awaitingTransitions;
    void doNextTransition();

    // Scene fitting and resizing
    void resizeSceneRect();
    bool uiHasBeenDrawn;

    // Keyboard in/out transtion
    QTimer keyboardTransitionTimer;
    qreal sceneDelta;
    qreal keyboardDelta;
    qint32 keyboardTransitionSteps;
    bool keyboardInTransition;

    //  Show/hide the soft keyboard
    void showSoftKeyboard();
    void hideKeyboard();

};

#endif // COLOUICONTAINER_H
