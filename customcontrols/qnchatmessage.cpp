#include "qnchatmessage.h"
#include <QFontMetrics>
#include <QPaintEvent>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QLabel>
#include <QDebug>

QNChatMessage::QNChatMessage(QWidget *parent) : QWidget(parent)
{
    QFont te_font = this->font();
    te_font.setFamily("MicrosoftYaHei");
    te_font.setPointSize(12);

    this->setFont(te_font);
    m_leftPixmap = QPixmap(":/img/Customer Copy.png");
    m_rightPixmap = QPixmap(":/img/CustomerService.png");

    m_loadingMovie = new QMovie(this);
    m_loadingMovie->setFileName(":/img/loading4.gif");
    m_loading = new QLabel(this);
    m_loading->setMovie(m_loadingMovie);
    m_loading->resize(16,16);
    m_loading->setAttribute(Qt::WA_TranslucentBackground , true);
    m_loading->setAutoFillBackground(false);
}

void QNChatMessage::setTextSuccess()
{
    m_loading->hide();
    m_loadingMovie->stop();
    m_isSending = true;
}

void QNChatMessage::setText(const QString &text, const QString &time, const QSize &allSize, const User_Type &userType)
{
    m_msg = text;
    m_userType = userType;
    m_time = time;
    // m_curTime = QDateTime::fromTime_t(time.toInt()).toString("hh:mm");
    m_curTime = time;
    m_allSize = allSize;

    //暂时不用这个功能
//    if(userType == User_Me)
//    {
//        if(!m_isSending)
//        {
//            m_loading->move(m_kuangRightRect.x() - m_loading->width() - 10, m_kuangRightRect.y()+m_kuangRightRect.height()/2- m_loading->height()/2);
//            m_loading->show();
//            m_loadingMovie->start();
//        }
//    }
//    else
//    {
//        m_loading->hide();
//    }

    this->update();
}

QSize QNChatMessage::fontRect(const QString &message, const QString &userName)
{
    m_msg = message;
    m_curUserName = userName;

    //以下注释以左侧为主
    int minHei = 30;        //最小高度
    int iconWH = 40;        //头像的宽和高
    int iconSpaceW = 20;    //头像的x坐标
    int iconTMPH = 10;      //头像的y坐标
    int iconRectW = 5;      //三角与头像的间距
    int sanJiaoW = 6;       //三角的宽度
    int kuangTMP = 20;
    int textSpaceRect = 12;

    //气泡的最大长度
    m_kuangWidth = this->width() - kuangTMP - 2 * (iconWH + iconSpaceW + iconRectW);
    m_textWidth = m_kuangWidth - 2 * textSpaceRect;
    m_spaceWid = this->width() - m_textWidth;

    //头像大小和起始位置
    m_iconLeftRect = QRect(iconSpaceW, iconTMPH, iconWH, iconWH);
    m_iconRightRect = QRect(this->width() - iconSpaceW - iconWH, iconTMPH, iconWH, iconWH);

    //获取文字大小
    QSize textSize = getRealString(m_msg);

    //文字高度
    int height = textSize.height() < minHei ? minHei : textSize.height();

    //三角大小和起始位置
    m_sanjiaoLeftRect = QRect(iconWH + iconSpaceW + iconRectW, m_lineHeight / 2, sanJiaoW, height - m_lineHeight);
    m_sanjiaoRightRect = QRect(this->width() - iconRectW - iconWH - iconSpaceW - sanJiaoW, m_lineHeight / 2, sanJiaoW, height - m_lineHeight);

    //气泡大小和起始位置（长信息与短信息唯一不同的只有宽度）
    if(textSize.width() < (m_textWidth + m_spaceWid))
    {
        //短信息
        m_kuangLeftRect.setRect(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), m_lineHeight / 4 * 3, textSize.width() - m_spaceWid + 2 * textSpaceRect, height - m_lineHeight);
        m_kuangRightRect.setRect(this->width() - textSize.width() + m_spaceWid - 2 * textSpaceRect - iconWH - iconSpaceW - iconRectW - sanJiaoW,
                                m_lineHeight / 4 * 3, textSize.width() - m_spaceWid + 2 * textSpaceRect, height - m_lineHeight);
    }
    else
    {
        //长信息
        m_kuangLeftRect.setRect(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), m_lineHeight / 4 * 3, m_kuangWidth, height - m_lineHeight);
        m_kuangRightRect.setRect(iconWH + kuangTMP + iconSpaceW + iconRectW - sanJiaoW, m_lineHeight / 4 * 3, m_kuangWidth, height - m_lineHeight);
    }

    //内容大小和起始位置
    m_textLeftRect.setRect(m_kuangLeftRect.x() + textSpaceRect, m_kuangLeftRect.y() + iconTMPH,
                           m_kuangLeftRect.width() - 2 * textSpaceRect, m_kuangLeftRect.height() - 2 * iconTMPH);
    m_textRightRect.setRect(m_kuangRightRect.x() + textSpaceRect, m_kuangRightRect.y() + iconTMPH,
                            m_kuangRightRect.width() - 2 * textSpaceRect, m_kuangRightRect.height() - 2 * iconTMPH);


    //获取用户名文本大小
    QFontMetrics fmUserName(this->font());
    QSize userNameSize = fmUserName.size(Qt::TextSingleLine, userName);

    //设置用户名文本矩形位置
    m_userLeftNameRect.setRect(m_iconLeftRect.x() + m_iconLeftRect.width() + iconRectW,
                               m_iconLeftRect.y(),
                               userNameSize.width(),
                               userNameSize.height());

    m_userRightNameRect.setRect(m_iconRightRect.x() - userNameSize.width() - iconRectW,
                                m_iconRightRect.y(),
                                userNameSize.width(),
                                userNameSize.height());

//    //更新气泡的y起始位置，确保有空间显示用户名
//    m_kuangLeftRect.setY(m_userLeftNameRect.bottom() + iconTMPH);
//    m_kuangRightRect.setY(m_userRightNameRect.bottom() + iconTMPH);

//    //更新文本的y起始位置
//    m_textLeftRect.setY(m_kuangLeftRect.y() + textSpaceRect);
//    m_textRightRect.setY(m_kuangRightRect.y() + textSpaceRect);

    return QSize(textSize.width(), height);
}

//获取文字大小
QSize QNChatMessage::getRealString(QString src)
{
    QFontMetricsF fm(this->font());

    //获取文本行间距
    m_lineHeight = fm.lineSpacing();

    //获取换行符数量
    int nCount = src.count("\n");
    int nMaxWidth = 0;
    if(nCount == 0)
    {
        //文本字符串的宽度
        nMaxWidth = fm.width(src);
        QString value = src;
        if(nMaxWidth > m_textWidth)
        {
            nMaxWidth = m_textWidth;
            int size = m_textWidth / fm.width(" ");
            int num = fm.width(value) / m_textWidth;
            num = ( fm.width(value) ) / m_textWidth;
            nCount += num;
            QString temp = "";
            for(int i = 0; i < num; i++)
            {
                temp += value.mid(i * size, (i + 1) * size) + "\n";
            }
            src.replace(value, temp);
        }
    }
    else
    {
        for(int i = 0; i < (nCount + 1); i++)
        {
            QString value = src.split("\n").at(i);
            nMaxWidth = fm.width(value) > nMaxWidth ? fm.width(value) : nMaxWidth;
            if(fm.width(value) > m_textWidth)
            {
                nMaxWidth = m_textWidth;
                int size = m_textWidth / fm.width(" ");
                int num = fm.width(value) / m_textWidth;
                num = ((i+num)*fm.width(" ") + fm.width(value)) / m_textWidth;
                nCount += num;
                QString temp = "";
                for(int i = 0; i < num; i++)
                {
                    temp += value.mid(i*size, (i+1)*size) + "\n";
                }
                src.replace(value, temp);
            }
        }
    }
    return QSize(nMaxWidth + m_spaceWid, (nCount + 1) * m_lineHeight + 2 * m_lineHeight);
}

void QNChatMessage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    if(m_userType == User_Type::User_OtherPerson)
    {
        //用户头像
        painter.drawPixmap(m_iconLeftRect, m_leftPixmap);

        //框加边
        QColor col_KuangB(234, 234, 234);
        painter.setBrush(QBrush(col_KuangB));
        painter.drawRoundedRect(m_kuangLeftRect.x() - 1, m_kuangLeftRect.y() - 1, m_kuangLeftRect.width() + 2, m_kuangLeftRect.height() + 2, 4, 4);

        //聊天气泡
        QColor col_Kuang(255, 255, 255);
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(m_kuangLeftRect, 4, 4);

        //三角
        QPointF points[3] = {
            QPointF(m_sanjiaoLeftRect.x(), 30),
            QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 25),
            QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 35),
        };
        QPen pen;
        pen.setColor(col_Kuang);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        //三角加边
        QPen penSanJiaoBian;
        penSanJiaoBian.setColor(col_KuangB);
        painter.setPen(penSanJiaoBian);
        painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 24));
        painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x() + m_sanjiaoLeftRect.width(), 36));

        //内容
        QPen penText;
        penText.setColor(QColor(51, 51, 51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(m_textLeftRect, m_msg, option);

        //在绘制头像后添加绘制用户名文本的代码
        QFont font = this->font();
        font.setPointSize(8);

        QPen penName;
        penName.setColor(QColor(51, 51, 51));
        painter.setPen(penName);
        painter.setFont(font);

        painter.drawText(m_userLeftNameRect, Qt::AlignBottom | Qt::AlignLeft, m_curUserName);
    }
    else if(m_userType == User_Type::User_Me)
    {
        //自己头像
        painter.drawPixmap(m_iconRightRect, m_rightPixmap);

        //聊天气泡
        QColor col_Kuang(75, 164, 242);
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(m_kuangRightRect, 4, 4);

        //三角
        QPointF points[3] = {
            QPointF(m_sanjiaoRightRect.x() + m_sanjiaoRightRect.width(), 30),
            QPointF(m_sanjiaoRightRect.x(), 25),
            QPointF(m_sanjiaoRightRect.x(), 35),
        };
        QPen pen;
        pen.setColor(col_Kuang);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        //内容
        QPen penText;
        penText.setColor(QColor(51, 51, 51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(m_textRightRect, m_msg, option);

        //在绘制头像后添加绘制用户名文本的代码
        QFont font = this->font();
        font.setPointSize(8);

        QPen penName;
        penName.setColor(QColor(51, 51, 51));
        painter.setPen(penName);
        painter.setFont(font);

        painter.drawText(m_userRightNameRect, Qt::AlignBottom | Qt::AlignRight, m_curUserName);
    }
    else if(m_userType == User_Type::User_Time)
    {
        //时间
        QPen penText;
        penText.setColor(QColor(153, 153, 153));
        painter.setPen(penText);
        QTextOption option(Qt::AlignCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        QFont te_font = this->font();
        te_font.setFamily("MicrosoftYaHei");
        te_font.setPointSize(10);
        painter.setFont(te_font);
        painter.drawText(this->rect(), m_curTime, option);
    }
}
