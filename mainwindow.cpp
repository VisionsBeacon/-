#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "application.h"
#include "data.h"

#include <QDebug>
#include <QHostAddress>
#include <QDateTime>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    initConnect();

    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::initConnect()
{
    connect(hApp->m_sigmanager, &SignalManager::connectSucceed, this, &MainWindow::onConnectSucceed);
    connect(hApp->m_sigmanager, &SignalManager::handleReceivedMessage, this, &MainWindow::onHandleReceivedMessage);
    connect(hApp->m_sigmanager, &SignalManager::handleLoginResult, this, &MainWindow::onHandleLoginResult);
    connect(hApp->m_sigmanager, &SignalManager::handleSingUpResult, this, &MainWindow::onHandleSingUpResult);
}

void MainWindow::onConnectSucceed(bool isConnected)
{
    if(isConnected)
    {
        ui->labConnect->setText("连接成功！");
        m_isConnect = true;
    }
    else
    {
        ui->labConnect->setText("连接失败！");
        m_isConnect = false;
    }
}

//打印在聊天框
void MainWindow::onHandleReceivedMessage(QString data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject messageObject = doc.object();

    QJsonObject messages = messageObject["messages"].toObject();

    QString username = messages["username"].toString();
    QString chatMessage = messages["chat"].toString();

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QString formattedMessage = QString("[%1] %2: %3").arg(timestamp, username, chatMessage);
    ui->editReceivedMessage->append(formattedMessage);
}

//登录返回
void MainWindow::onHandleLoginResult(int code)
{
    switch (code)
    {
        case Login_Successed:
        {
            QMessageBox::information(nullptr, "登录成功", "欢迎您！", QMessageBox::Ok);
            ui->stackedWidget->setCurrentIndex(1);
            break;
        }
        case Error_Password:
        {
            QMessageBox::information(nullptr, "登录失败", "密码错误", QMessageBox::Ok);
            break;
        }
        case Unknow_UserName:
        {
            QMessageBox::information(nullptr, "登录失败", "该用户名未注册", QMessageBox::Ok);
            break;
        }
        case Erro_Sql:
        {
            QMessageBox::information(nullptr, "登录失败", "网络故障", QMessageBox::Ok);
            break;
        }
        default:
            break;
    }
}

//注册返回
void MainWindow::onHandleSingUpResult(int code)
{
    switch (code)
    {
        case SignUp_Successed:
        {
            QMessageBox::information(nullptr, "注册成功", "注册成功，请重新登录", QMessageBox::Ok);
            break;
        }
        case Exist_UserName:
        {
            QMessageBox::information(nullptr, "注册失败", "已存在该用户", QMessageBox::Ok);
            break;
        }
        case Erro_Sql:
        {
            QMessageBox::information(nullptr, "注册失败", "网络故障", QMessageBox::Ok);
            break;
        }
        default:
            break;
    }
}

//连接服务端
void MainWindow::on_btnConnect_clicked()
{
    if(m_isConnect)
    {
        return;
    }

    Client::instance()->connectToServer(QHostAddress(ui->editAddress->text()), ui->editPort->text().toInt());
}

//发送数据
void MainWindow::on_pushButton_2_clicked()
{
    Client::instance()->writeMessage(ui->editMessage->text());
}

//登录
void MainWindow::on_btnLogIn_clicked()
{
    Client::instance()->sendLoginRequest(ui->editUserName->text(), ui->editUserPassword->text());
}

//注册账号
void MainWindow::on_btnSignUp_clicked()
{
    Client::instance()->sendSignUpRequest(ui->editUserName->text(), ui->editUserPassword->text());
}

//退出
void MainWindow::on_btnExit_clicked()
{
    close();
}

