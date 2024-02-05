#ifndef CLIENT_H
#define CLIENT_H


#include <QObject>
#include <QTcpSocket>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
    static Client *instance();
    void connectToServer(const QHostAddress &address, quint16 port);
    void writeMessage(const QString &message);
    void sendLoginRequest(const QString& username, const QString& password);
    void sendSignUpRequest(const QString& username, const QString& password);
    void processMessage(QByteArray data);
    void sendWidthLengthPrefix(QByteArray data);

signals:



private slots:
    void onConnected();
    void onReadyRead();
    void onErrorOccurred(QTcpSocket::SocketError socketError);

private:
    QTcpSocket *m_socket = nullptr;
    static Client* m_client;
    QString m_username;
    int m_currentBlockSize = 0;
};

#endif // CLIENT_H
