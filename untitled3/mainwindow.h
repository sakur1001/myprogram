#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define TIMEOUT 20
#include <QMainWindow>
#include<QPushButton>
#include<QMediaPlayer>
#include <QAudioOutput>
#include<QTimerEvent>

#include <QUrl>
#include <QMessageBox>
#include<QFileDialog>
#include<QTreeWidgetItem>
#include "pallet.h"
#include "client.h"
#include "chat.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Client *m_client;  // 声明m_client成员变量
    void setClient(Client *client);
    pallet*ppage3;//保存页面三地址
    void timerEvent(QTimerEvent*e);
    void initmusic();
    void onProfileUpdateResponse(const QJsonObject &userData); //用户资料

    void requestFriendList();  //请求联系人列表

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


    bool m_lottery;
    int timerid;
    QMediaPlayer*m_player;
    QAudioOutput *m_audioOutput;
    QStringList m_list;
    int m_pos;
    int m_userId;
    QString m_account;
    QString m_avatarPath;



    void initui();
    void Initlottery();

    void onFriendListReceived(const QJsonObject &friendList);  //列表构建
    QPixmap circlePixmap;
    void avatar(QString avatarPath,QLabel*label);
     Chat *m_anotherWidget;


};

#endif // MAINWINDOW_H
