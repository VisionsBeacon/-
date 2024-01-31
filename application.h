#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QApplication>

#include "signalmanager.h"

#include <QThread>

#define hApp (static_cast<Application *>(Application::instance()))



class Application : public QApplication
{
    Q_OBJECT

public:
    static Application* instance();
    static Application* instance(int &argc, char **argv);
    void init();
    explicit Application(int &argc, char **argv);

public:
    SignalManager* m_sigmanager= nullptr;

private:
    void initSettings();

private:
    static Application* m_singlenApplication;

};

#endif // APPLICATION_H
