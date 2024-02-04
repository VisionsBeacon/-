#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#include <QObject>


class SignalManager : public QObject
{
    Q_OBJECT

public:
    static SignalManager* instance();

signals:
    void connectSucceed(bool);
    void handleReceivedMessage(QString);
    void handleLoginResult(int);
    void handleSingUpResult(int);
    void updateOnlineUsers(QStringList);

private:
    explicit SignalManager(QObject *parent = nullptr);

private:
    static SignalManager* m_signalManager;
};

#endif // SIGNALMANAGER_H
