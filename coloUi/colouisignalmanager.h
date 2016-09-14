#ifndef COLOUISIGNALMANAGER_H
#define COLOUISIGNALMANAGER_H

#include <QObject>
#include "colouidefines.h"

class ColoUiSignalManager : public QObject
{
    Q_OBJECT
public:
    explicit ColoUiSignalManager(QObject *parent = 0, QString ID = "");
    void sendSignal(ColoUiSignalEventInfo sei);
    ColoUiSignalEventInfo getSignalEventInfo() const {return lastSignalTriggered;}


signals:
    void signalTriggered();

private:
    QString coloUiViewID;
    ColoUiSignalEventInfo lastSignalTriggered;
};

#endif // COLOUISIGNALMANAGER_H
