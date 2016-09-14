#include "colouisignalmanager.h"

ColoUiSignalManager::ColoUiSignalManager(QObject *parent, QString ID) : QObject(parent)
{
    coloUiViewID = ID;
}

void ColoUiSignalManager::sendSignal(ColoUiSignalEventInfo sei){
    sei.elementID = coloUiViewID + "." + sei.elementID;
    lastSignalTriggered = sei;
    emit signalTriggered();
}
