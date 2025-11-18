#include "client.h"

#include<QDateTime>
#include<QMessageBox>
Client::Client(QWidget *parent)
    : QWidget(parent)
    ,m_tcpSocket(new QTcpSocket(this))
{
    start("127.0.0.1",8888);

}

Client::~Client()
{
}





void Client::start(const QString &host, qint16 port)
{
    m_tcpSocket->connectToHost(QHostAddress(host),port);
    connect(m_tcpSocket,&QTcpSocket::disconnected,this,&Client::slot_disconnected);
    connect(m_tcpSocket,&QTcpSocket::errorOccurred,this,&Client::slot_errorOccurred);
    connect(m_tcpSocket,&QTcpSocket::readyRead,this,&Client::slot_readyRead);
    connect(m_tcpSocket,&QTcpSocket::connected,this,&Client::slot_connected);
}
void Client::slot_disconnected()
{
    qDebug()<<"disconnect ";
}

void Client::slot_errorOccurred(QAbstractSocket::SocketError socketError)
{
    qWarning()<<m_tcpSocket->errorString();
}


//接收处理信息
void Client::slot_readyRead()
{
    QByteArray data = m_tcpSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject message = doc.object();
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "收到无效的JSON数据";
        return;
    }

    // 根据消息类型处理
    QString msgType = message["type"].toString();

    if (msgType == "login_response") {
        LoginResponse(message);
    } else if (msgType == "register_response") {
        RegisterResponse(message);
    }  else if (msgType == "chat_message") {
        handleChatMessage(message);
    }else if (msgType == "friend_list_response") {
        handleFriendListResponse(message);
    } else if (msgType == "update_profile_response") {
        handleProfileUpdateResponse(message);
    } else if (msgType == "add_friend_response") {
        handleAddFriendResponse(message);
    }else if (msgType == "error") {
        QString errorMsg = message["message"].toString();
        qDebug() << "服务器错误:" << errorMsg;
        QMessageBox::warning(this, "错误", errorMsg);
    }
}

void Client::LoginResponse(const QJsonObject &response)
{
    bool success = response["success"].toBool();

    if (success) {
        QJsonObject userData = response["user_data"].toObject();

        // 保存到成员变量
        m_userData = userData;
        emit loginSuccess(m_userData);
    } else {

        QString reason = response["message"].toString();
        emit loginFailed(reason);
    }
}

void Client::RegisterResponse(const QJsonObject &response)
{
    bool success = response["success"].toBool();

    if (success) {

        emit registerSuccess();
    } else {
        QString reason = response["reason"].toString();
        emit registerFailed(reason);
    }
}



// 添加聊天消息处理方法
void Client::handleChatMessage(const QJsonObject &message)
{
    QString fromUser = message["from_user"].toString();
    QString content = message["content"].toString();
    QString timestamp = QDateTime::fromMSecsSinceEpoch(message["timestamp"].toVariant().toLongLong()).toString("hh:mm:ss");

    QString displayMsg = QString("[%1] %2: %3\n").arg(timestamp, fromUser, content);


    // 转发聊天消息信号
    emit chatMessageReceived(message);

    qDebug() << "收到聊天消息:" << fromUser << "->" << content;
}

void Client::handleFriendListResponse(const QJsonObject &response)
{
    // 处理服务器返回的好友列表
    emit friendListReceived(response);
}

void Client::handleAddFriendResponse(const QJsonObject &response)
{
    bool success = response["success"].toBool();
    QString message = response["message"].toString();

    if (success) {
        QMessageBox::information(this, "成功", message);
        qDebug() << "添加好友成功:" << message;
        // 添加好友成功后，直接构造一个好友列表请求发送给服务器
        QJsonObject friendListRequest;
        friendListRequest["type"] = "get_user_list";

        QJsonDocument doc(friendListRequest);
        m_tcpSocket->write(doc.toJson());
    } else {
        QMessageBox::warning(this, "失败", message);
        qDebug() << "添加好友失败:" << message;
    }
}

void Client::handleProfileUpdateResponse(const QJsonObject &response)
{
    bool success = response["success"].toBool();
    if (success) {
        QMessageBox::information(this, "成功", "资料更新成功");
        emit profileUpdateResponse(response);
    } else {
        QString reason = response["reason"].toString();
        QMessageBox::warning(this, "失败", "资料更新失败: " + reason);
    }
}


void Client::slot_connected()
{
    qDebug()<<"connect successful";
}
