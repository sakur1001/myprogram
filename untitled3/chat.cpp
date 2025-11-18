#include "chat.h"
#include "ui_chat.h"
#include <QMessageBox>

Chat::Chat(QTcpSocket* socket, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chat)
    , m_tcpSocket(socket)
{
    ui->setupUi(this);



}

Chat::~Chat()
{
    delete ui;
}

void Chat::on_pushButton_3_clicked()
{
    QString msgContent = ui->inputtextEdit->toPlainText().trimmed();
    if (msgContent.isEmpty()) {
        return;
    }

    // 构造JSON格式的聊天消息
    QJsonObject chatMsg;
    chatMsg["type"] = "chat_message";
    chatMsg["target_user"] = m_targetAccount;  // 使用成员变量
    chatMsg["content"] = msgContent;
    chatMsg["content_type"] = "text";
    chatMsg["timestamp"] = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // 发送JSON消息
    QJsonDocument doc(chatMsg);
    m_tcpSocket->write(doc.toJson());

    // 在界面显示自己发送的消息
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString displayMsg = QString("[%1] 我: %2\n").arg(timestamp, msgContent);
    ui->displaytextEdit->insertPlainText(displayMsg);
    ui->inputtextEdit->clear();
}

void Chat::setTargetName(const QString &name, const QString &Account)
{
    m_targetAccount = Account;
    m_targetName = name;
    ui->label_name->setText(name);
}

void Chat::onChatMessageReceived(const QJsonObject &message)
{
    QString fromUser = message["from_user"].toString();
    QString content = message["content"].toString();
    QString timestamp = QDateTime::fromMSecsSinceEpoch(message["timestamp"].toVariant().toLongLong()).toString("hh:mm:ss");

    // 只显示来自当前聊天对象的消息
    if (fromUser == m_targetAccount) {
        QString displayMsg = QString("[%1] %2: %3\n").arg(timestamp, fromUser, content);
        ui->displaytextEdit->insertPlainText(displayMsg);
    }
}
