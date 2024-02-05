#include "client.h"
#include "application.h"
#include "data.h"

#include <QDebug>
#include <QNetworkProxy>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <winsock2.h>
#include <ws2tcpip.h>

Client *Client::m_client =  nullptr;

Client::Client(QObject *parent)
    : QObject{parent}
{
    m_socket = new QTcpSocket(this);
    m_socket->setProxy(QNetworkProxy::NoProxy);

    connect(m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(m_socket, static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
            this, &Client::onErrorOccurred);

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0)
    {
        //处理错误
    }
}

Client::~Client()
{
    WSACleanup();
}

Client *Client::instance()
{
    if(nullptr == m_client)
    {
        m_client = new Client();
    }

    return m_client;
}

void Client::connectToServer(const QHostAddress &address, quint16 port)
{
    qDebug() << "连接服务器：" << address << port;
    m_socket->connectToHost(address, port);
}

void Client::onConnected()
{
    Q_EMIT hApp->m_sigmanager->connectSucceed(true);
}

//接收服务端数据
void Client::onReadyRead()
{
    while(m_socket->bytesAvailable())
    {
        //获取消息长度（4B）
        if(m_currentBlockSize == 0)
        {
            //qint64表示一个64位的有符号整数
            if(m_socket->bytesAvailable() < static_cast<qint64>(sizeof(uint32_t)))
            {
                return;
            }
            else
            {
                QByteArray sizeBytes = m_socket->read(sizeof(uint32_t));
                uint32_t length = ntohl(*reinterpret_cast<const uint32_t*>(sizeBytes.data()));
                m_currentBlockSize = length;
            }
        }

        if (m_socket->bytesAvailable() < m_currentBlockSize)
        {
            //等待更多数据
            return;
        }

        QByteArray message = m_socket->read(m_currentBlockSize);
        m_currentBlockSize = 0;

        //处理消息
        processMessage(message);
    }
}

void Client::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);

    Q_EMIT hApp->m_sigmanager->connectSucceed(false);

    qDebug() << "Connection failed:" << m_socket->errorString();
}

//客户端发送聊天内容
void Client::writeMessage(const QString &message)
{
    QJsonObject messagesObject;
    messagesObject["username"] = m_username;
    messagesObject["chat"] = message;

    QJsonObject messageObject;
    messageObject["type"] = Normal;
    messageObject["messages"] = messagesObject;

    QByteArray data = QJsonDocument(messageObject).toJson(QJsonDocument::Compact);

    sendWidthLengthPrefix(data);
}

//客户端发送登录请求
void Client::sendLoginRequest(const QString& username, const QString& password)
{
    m_username = username;

    QJsonObject messagesObject;
    messagesObject["username"] = username;
    messagesObject["password"] = password;

    QJsonObject loginRequestObject;
    loginRequestObject["type"] = Login;
    loginRequestObject["messages"] = messagesObject;

    QByteArray data = QJsonDocument(loginRequestObject).toJson();
    sendWidthLengthPrefix(data);
}

//客户端发送注册请求
void Client::sendSignUpRequest(const QString &username, const QString &password)
{
    QJsonObject messagesObject;
    messagesObject["username"] = username;
    messagesObject["password"] = password;

    QJsonObject loginRequestObject;
    loginRequestObject["type"] = signUp;
    loginRequestObject["messages"] = messagesObject;

    QByteArray data = QJsonDocument(loginRequestObject).toJson();
    sendWidthLengthPrefix(data);
}

void Client::processMessage(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    int messageType = obj["type"].toInt();
    QJsonObject messagesObj = obj["messages"].toObject();


    QString dataStr = QString::fromUtf8(data.data());
    dataStr.replace("\n", "").replace("\t", "");
    qDebug() << "Received JSON:" << dataStr;


    switch (messageType)
    {
        case LoginResult:
        {
            int code = messagesObj["code"].toInt();
            QString userName = messagesObj["username"].toString();
            Q_EMIT hApp->m_sigmanager->handleLoginResult(code, userName);
            break;
        }
        case SignupResult:
        {
            int code = messagesObj["code"].toInt();
            Q_EMIT hApp->m_sigmanager->handleSingUpResult(code);
            break;
        }
        case Normal:
        {
            QString message = QString::fromUtf8(data);
            Q_EMIT hApp->m_sigmanager->handleReceivedMessage(message);
            break;
        }
        case OnlineUsers:
        {
            QJsonArray usernames = messagesObj["usernames"].toArray();
            QStringList usernameList;
            for (const QJsonValue &val : usernames)
            {
                usernameList.append(val.toString());
            }

            Q_EMIT hApp->m_sigmanager->updateOnlineUsers(usernameList);
        }
        default:
            break;
    }
}

void Client::sendWidthLengthPrefix(QByteArray data)
{
    uint32_t dataLength = static_cast<uint32_t>(data.size());
    uint32_t netLength = htonl(dataLength);

    m_socket->write(reinterpret_cast<const char*>(&netLength), sizeof(netLength));
    m_socket->write(data);
}
