#include "application.h"
#include "controller/signalmanager.h"

#include <QThread>
#include <QDir>

Application* Application::m_singlenApplication = nullptr;

Application::Application(int &argc, char **argv)
    : QApplication(argc,argv)
{
    init();
}

Application *Application::instance()
{
    return m_singlenApplication;
}

Application *Application::instance(int &argc, char **argv)
{
    if(m_singlenApplication == nullptr)
    {
        m_singlenApplication = new Application(argc,argv);
    }
    return m_singlenApplication;
}

void Application::init()
{
    m_sigmanager = SignalManager::instance();
}
