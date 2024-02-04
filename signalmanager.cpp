#include "signalmanager.h"

SignalManager* SignalManager::m_signalManager = nullptr;

SignalManager::SignalManager(QObject *parent) : QObject(parent)
{

}

SignalManager *SignalManager::instance()
{
    if(!m_signalManager)
    {
        m_signalManager = new SignalManager();
    }
    return m_signalManager;
}
