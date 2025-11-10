#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define TIMEOUT 20
#include <QMainWindow>
#include<QPushButton>
#include<QMediaPlayer>
#include <QAudioOutput>
#include<QTimerEvent>
#include <QSqlQuery>
#include <QSqlError>
#include <QUrl>
#include <QMessageBox>
#include<QFileDialog>
#include<QTreeWidgetItem>
#include "pallet.h"
#include "client.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    pallet*ppage3=NULL;//保存页面三地址
    void InitALL(const QSqlDatabase &db,int userId,QString account);
    void timerEvent(QTimerEvent*e);
    void initmusic();


signals:
    void back();

private slots:

    void on_back_pushButton_clicked();

    void on_play_pushButton_clicked();

    void on_musicButton_clicked();

    void on_lotteryButton_clicked();

    void on_personalButton_clicked();

    void on_listButton_clicked();


    void on_lotteryplayButton_clicked();



    void on_pushButton_4_clicked();


    void on_dataButton_clicked();

    void on_submitButton_clicked();

    void on_dataBackButton_clicked();

    void on_chooseAvatarButton_clicked(); // 选择头像按钮的槽函数

    void on_comboBox_activated(int index);

    void on_pushButton_5_clicked();

    void on_pushButton_clicked();

    void on_friendTree_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    int getGroupIdByGroupName(const QString &groupName);

    bool m_lottery;
    int timerid;
    QMediaPlayer*m_player;
    QAudioOutput *m_audioOutput;
    QStringList m_list;
    int m_pos;
    int m_userId;
    QString m_account;
    QString m_avatarPath;
    QSqlDatabase m_db;
    void initui();
    void Initlottery();
    void InitData();
    void listfriend();
    QPixmap circlePixmap;
    void avatar(QString avatarPath,QLabel*label);
    Client *m_anotherWidget;


};

#endif // MAINWINDOW_H
