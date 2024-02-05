#ifndef DATA_H
#define DATA_H

enum LoginResultType
{
    Login_Successed,    //登录成功
    SignUp_Successed,   //注册成功
    Error_Password,     //密码错误
    Unknow_UserName,    //不存在该用户
    Exist_UserName,     //已存在该用户
    AlreadyLoggedIn,    //该用户已登录
    Erro_Sql            //网络错误
};

enum MessageType
{
    Login,
    signUp,
    Normal,
    LoginResult,
    SignupResult,
    OnlineUsers     //在线人数
};

#endif // DATA_H
