#include "client.h"
#include "ui_client.h"
#include<QDateTime>
Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
}

Client::~Client()
{
    delete ui;
}

void Client::on_pushButton_3_clicked()
{
    auto msg=ui->inputtextEdit->toPlainText();
    // auto data=QDateTime::currentDateTime().toString();
    ui->displaytextEdit->insertPlainText(m_name+" : "+msg+"\n");
    ui->inputtextEdit->clear();
}

void Client::setTargetName(const QString &name,QString &MyName)
{
    m_name=MyName;
    // 假设弹窗的.ui文件中有一个名为label_name的QLabel
    ui->label_name->setText(name);
}
