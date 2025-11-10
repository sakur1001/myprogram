#include "widget.h"
#include "./ui_widget.h"
//#include "mainwindow.h"
#include<QRegularExpressionValidator>
#include <QPushButton>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initui();
    initsql();

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

void Widget::initsql()
{
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    // db.setPort(3306);
    db.setDatabaseName("user_db");
    db.setUserName("root");
    db.setPassword("123456");
    db.open();
}


void Widget::on_login_clicked()//登录账号
{
    QString account=ui->account_lineEdit->text().trimmed();
    QString password=ui->password_lineEdit->text().trimmed();
    if (account.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "登录提示", "账号或密码不能为空！");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM user WHERE account = :account AND password = :password");
    query.bindValue(":account", account);
    query.bindValue(":password", password);
    if (query.exec() && query.next()) {
        userId = query.value("id").toInt();

        this->ppage2=new MainWindow;//实例化页面二



        this->ppage2->InitALL(db,userId,account); // 传递数据库连接

        connect(this->ppage2, &MainWindow::back, this, &Widget::on_back_pushButton_clicked);
        this->hide();
        this->ppage2->show();

    } else {
        QMessageBox::warning(this, "登录提示", "账号或密码错误！");
    }
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
    if (checkAccountExists(account)) {
        QMessageBox::warning(this, "注册提示", "该账号已存在！");
        ui->reg_account_lineEdit->clear();
        return;
    }

    // 插入数据库
    QSqlQuery query(db);
    query.prepare("INSERT INTO user (account, password) VALUES (:account, :password)");
    query.bindValue(":account", account);
    query.bindValue(":password", password); // 注意：实际项目需加密密码，此处为演示用明文
    if (query.exec()) {
        QMessageBox::information(this, "注册提示", "注册成功！");
        ui->stackedWidget->setCurrentWidget(ui->log_page); // 跳转到登录页
        ui->reg_account_lineEdit->clear();
        ui->reg_password_lineEdit->clear();
        ui->again_password_lineEdit->clear();
    }
    qDebug()<<__FUNCTION__<<"username:"<<account<<password;
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
    // this->ppage2->hide();
    delete this->ppage2;
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




bool Widget::checkAccountExists(const QString &account)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM user WHERE account = :account");
    query.bindValue(":account", account);
    return query.exec() && query.next();
}

void Widget::regcjeck()
{

}

