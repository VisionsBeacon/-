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

const QString SERVERADDRESS = "10.5.10.121";
const int SERVERPORT = 1234;


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

    ui->stackedWidget->setCurrentIndex(2);

    connectToServer();
}

void MainWindow::initConnect()
{
    connect(hApp->m_sigmanager, &SignalManager::connectSucceed, this, &MainWindow::onConnectSucceed);
    connect(hApp->m_sigmanager, &SignalManager::handleReceivedMessage, this, &MainWindow::onHandleReceivedMessage);
    connect(hApp->m_sigmanager, &SignalManager::handleLoginResult, this, &MainWindow::onHandleLoginResult);
    connect(hApp->m_sigmanager, &SignalManager::handleSingUpResult, this, &MainWindow::onHandleSingUpResult);
    connect(hApp->m_sigmanager, &SignalManager::updateOnlineUsers, this, &MainWindow::onUpdateOnlineUsers);
}

void MainWindow::connectToServer()
{
    if(m_isConnect)
    {
        return;
    }

    Client::instance()->connectToServer(QHostAddress(SERVERADDRESS), SERVERPORT);
}

void MainWindow::onConnectSucceed(bool isConnected)
{
    if(isConnected)
    {
        m_isConnect = true;
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        QMessageBox::information(nullptr, "", "请检查网络或者服务器是否连接", QMessageBox::Ok);
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
        case AlreadyLoggedIn:
        {
            QMessageBox::information(nullptr, "登录失败", "该用户已登录", QMessageBox::Ok);
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

//更新在线人数
void MainWindow::onUpdateOnlineUsers(QStringList usernameList)
{
    //清空树和栈
    ui->treeWidget->clear();

    //设置表头
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setColumnHidden(1, true);

    ui->treeWidget->setHeaderLabels(QStringList() << "当前在线用户" );
    QString headerStyleSheet = "QHeaderView::section { font-size: 15px; }";
    ui->treeWidget->setStyleSheet(headerStyleSheet);
    //调整表头长度相同
    QHeaderView *header = ui->treeWidget->header();
    header->setSectionResizeMode(QHeaderView::Stretch);

    QTreeWidgetItem *groupItem = new QTreeWidgetItem(QStringList() << "所有成员");
    ui->treeWidget->addTopLevelItem(groupItem);

    for(const QString &userName : usernameList)
    {
        //显示一个用户
        QTreeWidgetItem* userItem = new QTreeWidgetItem(QStringList() << userName);
        userItem->setFont(0, QFont("Arial", 14));

        groupItem->addChild(userItem);
    }


    //更新在线人数
    ui->labCurrentUserCount->setText(QString::number(usernameList.size()));
}

//发送数据
void MainWindow::on_pushButton_2_clicked()
{
    Client::instance()->writeMessage(ui->editMessage->text());

    ui->editMessage->clear();
}

//登录
void MainWindow::on_btnLogIn_clicked()
{
    if(ui->editUserName->text().isEmpty() || ui->editUserPassword->text().isEmpty())
    {
        QMessageBox::information(nullptr, "注册失败", "账号或密码不能为空", QMessageBox::Ok);
        return;
    }

    Client::instance()->sendLoginRequest(ui->editUserName->text(), ui->editUserPassword->text());
}

//注册账号
void MainWindow::on_btnSignUp_clicked()
{
    if(ui->editUserName->text().isEmpty() || ui->editUserPassword->text().isEmpty())
    {
        QMessageBox::information(nullptr, "注册失败", "账号或密码不能为空", QMessageBox::Ok);
        return;
    }

    Client::instance()->sendSignUpRequest(ui->editUserName->text(), ui->editUserPassword->text());
}

//退出
void MainWindow::on_btnExit_clicked()
{
    close();
}

