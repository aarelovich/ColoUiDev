#include "colouisignalmanager.h"

ColoUiSignalManager::ColoUiSignalManager(QObject *parent) : QObject(parent)
{
}

void ColoUiSignalManager::sendSignal(ColoUiSignalEventInfo sei){
    lastSignalTriggered = sei;
    emit signalTriggered();
}
