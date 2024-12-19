#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "data.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidgetItem>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void initConnect();
    void connectToServer();
    void dealMessageTime(const QString &curMsgTime);
    void dealMessage(const ChatMessage &chatMessage);
    bool isIntervalMoreThanThreeMinutes(const QString &lastTime, const QString &curMsgTime);

private slots:
    void onConnectSucceed(bool);
    void onHandleReceivedMessage(QString data);
    void onHandleLoginResult(int code, QString userName);
    void onHandleSingUpResult(int code);
    void onUpdateOnlineUsers(QStringList usernameList);

    void on_pushButton_2_clicked();

    void on_btnLogIn_clicked();

    void on_btnSignUp_clicked();

    void on_btnExit_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    bool m_isConnect = false;
    QString m_currentUser;
};

#endif // MAINWINDOW_H
