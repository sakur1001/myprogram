#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    void setTargetName(const QString &name,QString &MyName);



private slots:
    void on_pushButton_3_clicked();

private:
    Ui::Client *ui;
    QString m_name;

};

#endif // CLIENT_H
