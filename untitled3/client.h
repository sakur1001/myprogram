#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include<QTcpSocket>
#include <QJsonObject>   // JSON 对象
#include <QJsonDocument> // JSON 文档（序列化/反序列化）
#include <QJsonValue>    // JSON 值
#include<QJsonArray>

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    QTcpSocket*m_tcpSocket;
    void start(const QString &host, qint16 port);
    QJsonObject m_userData;
signals:
    void loginSuccess(const QJsonObject &userData);
    void loginFailed(QString reason);
    void registerSuccess();
    void registerFailed(QString reason);
    void friendListReceived(const QJsonObject &response);  //联系人列表传回
    void profileUpdateResponse(const QJsonObject &response);  //用户资料传回
    // void messageReceived(const QJsonObject &response);//消聊天息接收传回
    void chatMessageReceived(const QJsonObject &message);  // 聊天消息接收传回
    void addFriendRequest(const QString &nickname, const QString &groupName);//好友列表响应传回

    void updateProfileRequest(const QJsonObject &profileData);
private slots:
    void slot_disconnected();
    void slot_errorOccurred(QAbstractSocket::SocketError socketError);
    void slot_readyRead();
    void slot_connected();
private:


    void LoginResponse(const QJsonObject &response);
    void RegisterResponse(const QJsonObject &response);

    void handleChatMessage(const QJsonObject &message);
    void handleFriendListResponse(const QJsonObject &response);  //联系人
    void handleProfileUpdateResponse(const QJsonObject &response);  // 用户资料更新
    void handleAddFriendResponse(const QJsonObject &response);  // 处理添加好友响应
};

#endif // CLIENT_H
