#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT
public:
    explicit Chat(QTcpSocket* socket, QWidget *parent = nullptr);  // 修改构造函数
    ~Chat();
    void setTargetName(const QString &name, const QString &Account);
    void onChatMessageReceived(const QJsonObject &message);
private slots:
    void on_pushButton_3_clicked();  // 发送消息


private:
    Ui::Chat *ui;
    QTcpSocket* m_tcpSocket;  // 使用socket而不是Client
    QString m_targetName;
    QString m_targetAccount;
};


#endif // CHAT_H
