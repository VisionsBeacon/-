#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>


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

private slots:
    void onConnectSucceed(bool);
    void onHandleReceivedMessage(QString data);
    void onHandleLoginResult(int code);
    void onHandleSingUpResult(int code);
    void onUpdateOnlineUsers(QStringList usernameList);

    void on_pushButton_2_clicked();

    void on_btnLogIn_clicked();

    void on_btnSignUp_clicked();

    void on_btnExit_clicked();

private:
    Ui::MainWindow *ui;
    bool m_isConnect = false;
};

#endif // MAINWINDOW_H
