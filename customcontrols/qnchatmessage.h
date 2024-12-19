#ifndef QNCHATMESSAGE_H
#define QNCHATMESSAGE_H

#include <QWidget>

class QPaintEvent;
class QPainter;
class QLabel;
class QMovie;

class QNChatMessage : public QWidget
{
    Q_OBJECT
public:
    explicit QNChatMessage(QWidget *parent = nullptr);

    enum User_Type{
        User_Unknow,
        User_System,//系统
        User_Me,    //自己
        User_OtherPerson,   //用户
        User_Time   //时间
    };
    void setTextSuccess();
    void setText(const QString &text, const QString &time, const QSize &allSize, const User_Type &userType);

    //获取文字大小
    QSize getRealString(QString src);
    QSize fontRect(const QString &message, const QString &userName);

    inline QString text() {return m_msg;}
    inline QString time() {return m_time;}
    inline User_Type userType() {return m_userType;}
protected:
    void paintEvent(QPaintEvent *event);
private:
    QString m_msg;
    QString m_time;
    QString m_curTime;
    QString m_curUserName;

    QSize m_allSize;
    User_Type m_userType = User_System;

    int m_kuangWidth;
    int m_textWidth;
    int m_spaceWid;
    int m_lineHeight;   //行间距

    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    QRect m_sanjiaoLeftRect;
    QRect m_sanjiaoRightRect;
    QRect m_kuangLeftRect;
    QRect m_kuangRightRect;
    QRect m_textLeftRect;
    QRect m_textRightRect;
    QRect m_userLeftNameRect;    //左侧用户名
    QRect m_userRightNameRect;   //右侧用户名
    QPixmap m_leftPixmap;
    QPixmap m_rightPixmap;
    QLabel* m_loading = Q_NULLPTR;
    QMovie* m_loadingMovie = Q_NULLPTR;
    bool m_isSending = false;
};

#endif // QNCHATMESSAGE_H
