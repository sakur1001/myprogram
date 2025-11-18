#include "widget.h"
#include "./ui_widget.h"

#include<QRegularExpressionValidator>
#include <QPushButton>
#include <QInputDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_client(new Client(this))  // 直接创建 Client
    , mainWin(nullptr)  // 初始化 mainWin
{
    ui->setupUi(this);
    initui();



    connect(m_client, &Client::loginSuccess, this, &Widget::onloginSuccess);
    connect(m_client, &Client::loginFailed, this, &Widget::onloginFailed);
    connect(m_client, &Client::registerSuccess, this, &Widget::onregisterSuccess);
    connect(m_client, &Client::registerFailed, this, &Widget::onregisterFailed);

    this->installEventFilter(new DragWidgetFilter(this));//过滤器鼠标事件


}

Widget::~Widget()
{
    delete ui;
}

void Widget::initui()
{
    ui->stackedWidget->setCurrentWidget(ui->log_page);
    ui->login->setEnabled(false);
    ui->account_lineEdit->setMaxLength(12);
    ui->password_lineEdit->setMaxLength(12);
    ui->password_lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$")));
    ui->reg_account_lineEdit->setMaxLength(12);
    ui->reg_password_lineEdit->setMaxLength(12);
    ui->again_password_lineEdit->setMaxLength(12);
    ui->reg_password_lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$")));
    ui->again_password_lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$")));


    connect(ui->account_lineEdit, &QLineEdit::textChanged, this, &Widget::on_radioButton_clicked);
    connect(ui->password_lineEdit, &QLineEdit::textChanged, this, &Widget::on_radioButton_clicked);


    setWindowFlag(Qt::FramelessWindowHint);//无标框
    setAttribute(Qt::WA_TranslucentBackground);//窗口透明


}


void Widget::on_login_clicked()//登录账号
{
    QString account=ui->account_lineEdit->text().trimmed();
    QString password=ui->password_lineEdit->text().trimmed();
    if (account.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "登录提示", "账号或密码不能为空！");
        return;
    }
    // 构造登录消息
    QJsonObject loginMsg;
    loginMsg["type"] = "login";
    loginMsg["account"] = account;
    loginMsg["password"] = password;
    loginMsg["timestamp"] = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // 发送登录请求
    QJsonDocument doc(loginMsg);
    m_client->m_tcpSocket->write(doc.toJson());
}


void Widget::onloginSuccess(const QJsonObject &userData)
{
    // 登录成功，跳转到主界面
    mainWin = new MainWindow();  // 传递同一个客户端实例
    mainWin->setClient(m_client);  // 设置客户端
    // 传递用户数据并初始化界面
    mainWin->onProfileUpdateResponse(userData);

    // 请求好友列表
    mainWin->requestFriendList();
    mainWin->show();
    this->hide();
    // 连接返回信号
    connect(mainWin, &MainWindow::back, this, &Widget::on_back_pushButton_clicked);
}

void Widget::onloginFailed(const QString &reason){
    QMessageBox::warning(this, "登录失败", reason);
    ui->password_lineEdit->clear();
    ui->password_lineEdit->setFocus();
}


void Widget::on_confirm_pushButton_clicked()//注册账号
{
    QString account=ui->reg_account_lineEdit->text().trimmed();
    QString password=ui->reg_password_lineEdit->text().trimmed();
    QString againpwd=ui->again_password_lineEdit->text().trimmed();
    // 输入校验
    if (account.isEmpty()) {
        QMessageBox::warning(this, "注册提示", "账号不能为空！");
        return;
    }
    if (password.isEmpty() || againpwd.isEmpty()) {
        QMessageBox::warning(this, "注册提示", "密码不能为空！");
        return;
    }
    if (password != againpwd) {
        QMessageBox::warning(this, "注册提示", "两次密码输入不一致！");
        ui->reg_password_lineEdit->clear();
        ui->again_password_lineEdit->clear();
        return;
    }

    // 构造注册消息
    QJsonObject registerMsg;
    registerMsg["type"] = "register";
    registerMsg["account"] = account;
    registerMsg["password"] = password;
    registerMsg["againpwd"] = againpwd;
    registerMsg["timestamp"] = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // 发送注册请求
    QJsonDocument doc(registerMsg);
    m_client->m_tcpSocket->write(doc.toJson());
}

void Widget::onregisterSuccess()
{
    QMessageBox::information(this, "注册成功", "账号注册成功，请登录！");
    ui->stackedWidget->setCurrentWidget(ui->log_page);
    // 清空注册表单
    ui->reg_account_lineEdit->clear();
    ui->reg_password_lineEdit->clear();
    ui->again_password_lineEdit->clear();

}
void Widget::onregisterFailed(const QString &reason)
{
    QMessageBox::warning(this, "注册失败", reason);
    ui->reg_password_lineEdit->clear();
    ui->again_password_lineEdit->clear();

}

void Widget::on_radioButton_clicked()
{
    if(ui->radioButton->isChecked()&&!ui->account_lineEdit->text().isEmpty()&&!ui->password_lineEdit->text().isEmpty())
    {
        ui->login->setStyleSheet("background-color:rgb(0,141,235)");
        ui->login->setEnabled(true);
    }
    else{
        ui->login->setStyleSheet("background-color:rgb(158,218,255)");
        ui->login->setEnabled(false);
    }

}


void Widget::on_close_toolButton_clicked()
{
    this->close();
}

void Widget::on_back_pushButton_clicked()
{
    delete this->mainWin;
    this->show();
}
void Widget::on_regback_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->log_page);
}



void Widget::on_regist_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->reg_page);
}


void Widget::regcjeck()
{

}

