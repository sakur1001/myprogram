#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client(nullptr)  // 初始化 m_client
    , m_anotherWidget(nullptr)  // 初始化 m_anotherWidget
{
    ui->setupUi(this);

    initui();
    Initlottery();
    initmusic();

    this->installEventFilter(new DragWidgetFilter(this));
    this->ppage3=new pallet();//实例化页面三


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setClient(Client *client)
{
    m_client = client;
    if (m_client) {
        connect(m_client, &Client::friendListReceived, this, &MainWindow::onFriendListReceived);
        connect(m_client, &Client::profileUpdateResponse, this, &MainWindow::onProfileUpdateResponse);
    }
}
void MainWindow::initui()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);//透明窗
    // setAttribute(Qt::WA_StyledBackground, true);

    ui->centralwidget->setStyleSheet(R"(
    QWidget{
        background:transparent;
        border:none;
    }
    QWidget#widget1{
        border-image:url(:/background/1002.png);
        border-top-left-radius: 0;    /* 左上角直角 */
        border-bottom-left-radius: 0; /* 左下直角 */
        border-top-right-radius:15px;      /* 右上角半圆 */
        border-bottom-right-radius: 15px;   /* 右下角半圆 */
    }

    QToolButton#toolButton{
    border-image :url(:/material/黑色关闭.svg);
    }

    QToolButton#toolButton:hover{
    background-color:rgba(230, 0, 0,180);
    border-top-right-radius: 15px;
    border-image :url(:/material/白色关闭.svg);
    }

    QToolButton#toolButton_2{
    border-image :url(:/material/设置.svg);
    }


    )");

    ui->widget->setStyleSheet(R"(
    QWidget#widget{
        border-top-left-radius: 15px;    /* 左上角半圆 */
        border-bottom-left-radius: 15px; /* 左下角半圆 */
        border-top-right-radius: 0;      /* 右上角直角 */
        border-bottom-right-radius: 0;   /* 右下角直角 */
        background-color:rgb(65, 85, 146);}
    QPushButton:hover {          /* 鼠标悬停状态 */
        background-color:rgba(50, 180, 245,80);}
    QPushButton:pressed {        /* 点击状态 */
        background-color:rgba(46, 157, 232,150);}


    )");

    ui->chooseAvatarButton->setStyleSheet(R"(
    QPushButton {
    border-radius: 50px;      /* 半径=宽/2（100/2=50），确保圆形 */

    }
    )");

    ui->stackedWidget->setCurrentWidget(ui->page3);
    ui->play_pushButton->setIcon(QIcon("://material/暂停.svg"));


    // 1. 创建主菜单
     QMenu *toolMenu = new QMenu(this);

        toolMenu->setStyleSheet(R"(
        QMenu {
            background-color: white;
            border: 1px solid #E5E5E5;
            border-radius: 6px;
            padding: 4px 0;
            font-size: 14px;
        }
        QMenu::item {
            padding: 6px 24px;
            margin: 0 4px;
        }
        QMenu::item:hover {
            background-color: #F5F7FA;
            color: #1890FF;
            border-radius: 4px;
        }
        QMenu::separator {
            height: 1px;
            background-color: #E5E5E5;
            margin: 4px 0;
        }
    )");
    QAction *actMin =toolMenu->addAction(QIcon(":/icons/min.svg"), "最小化");
    QAction *actPet = toolMenu->addAction(QIcon(":/icons/pet.svg"), "变身桌宠");
    QAction *actLogout = toolMenu->addAction(QIcon(":/icons/logout.svg"), "退出登录");
    QAction *actAbout = toolMenu->addAction(QIcon(":/icons/about.svg"), "关于软件");
    ui->toolButton_2->setMenu(toolMenu);
    ui->toolButton_2->setPopupMode(QToolButton::InstantPopup);
    ui->toolButton_2->installEventFilter(new MenuFilter(this));
    connect(actMin, &QAction::triggered, this, &Widget::showMinimized);
}



void MainWindow::Initlottery()
{
    m_list.push_back("上\n上\n签");
    m_list.push_back("上\n吉\n签");
    m_list.push_back("中\n吉\n签");
    m_list.push_back("中\n平\n签");
    m_list.push_back("下\n吉\n签");
    m_list.push_back("下\n下\n签");
    m_lottery=false;//抽奖开始初始化
    ui->lotteryplayButton->setIcon(QIcon("://material/暂停.svg"));
    ui->lotteryplayButton->setIconSize(ui->lotteryplayButton->size());
    m_pos=0;
}



void MainWindow::timerEvent(QTimerEvent*e)
{
    if(timerid==e->timerId())
    {
        m_pos++;
        if(m_pos>m_list.size()-1)
        {
            m_pos=0;
        }
    }
    ui->label->setText(m_list.at(m_pos));
}

void MainWindow::on_back_pushButton_clicked()
{
    emit this->back();
}


void MainWindow::on_play_pushButton_clicked()
{

    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause(); // 正在播放 → 暂停
        ui->play_pushButton->setIcon(QIcon("://material/暂停.svg"));


    } else {
        m_player->play();  // 暂停/停止 → 播放
        ui->play_pushButton->setIcon(QIcon("://material/播放.svg"));

    }

}


void MainWindow::on_musicButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page1);
}

void MainWindow::on_lotteryButton_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->page2);
}

void MainWindow::on_personalButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page3);
}

void MainWindow::on_listButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page4);
}

void MainWindow::on_dataButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page5);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page6);
}


void MainWindow::initmusic()
{
    m_player=new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput;
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.2); // 音量设为最大（0.0~1.0）
    m_player->setSource(QUrl("qrc:/music/小阿七-从前说.mp3"));
    m_player->setLoops(QMediaPlayer::Infinite); // 传入Infinite表示无限循环

}


void MainWindow::on_lotteryplayButton_clicked()//抽奖播放
{
    if (m_lottery) {
    m_lottery=false;
    ui->lotteryplayButton->setIcon(QIcon("://material/暂停.svg"));
    this->killTimer(timerid);


    } else {
    m_lottery=true;
    ui->lotteryplayButton->setIcon(QIcon("://material/播放.svg"));
    timerid=this->startTimer(TIMEOUT);
    }

}





void MainWindow::on_pushButton_4_clicked()
{
    this->hide();
    this->ppage3->show();
    this->ppage3->startMusicAndAnimation();
}


void MainWindow::on_chooseAvatarButton_clicked()
{

    // 打开文件选择对话框，过滤图片格式
    QString avatarPath = QFileDialog::getOpenFileName(
        this,
        "选择头像",       // 对话框标题
        "",              // 默认打开路径（空表示系统默认）
        "图片文件 (*.png *.jpg *.jpeg *.bmp)" // 仅显示图片格式文件
        );

    if (!avatarPath.isEmpty()) {
        m_avatarPath = avatarPath; // 保存用户选择的图片路
         avatar(avatarPath,ui->label_7);
    }
}

void MainWindow::on_submitButton_clicked()  //更改用户资料
{
    QString user_name=ui->user_nameEdit->text().trimmed();
    int level = 0;
    auto avatar = m_avatarPath;
    int age = ui->agelineEdit->text().trimmed().toInt();
    QString birthday = ui->birthdaylineEdit->text().trimmed();
    QString signature = ui->signatureEdit->toPlainText().trimmed();
    // 构造用户资料消息
    QJsonObject user_profileMsg;
    user_profileMsg["type"] = "update_profile";
    user_profileMsg["username"] = user_name;
    user_profileMsg["level"] = level;
    user_profileMsg["avatar"] = avatar;
    user_profileMsg["signature"] = signature;
    user_profileMsg["age"] = age;
    user_profileMsg["birthday"] = birthday;
    // 发送
    QJsonDocument doc(user_profileMsg);
    m_client->m_tcpSocket->write(doc.toJson());
}
void MainWindow::avatar(QString avatarPath,QLabel* label)
{
    QPixmap originalPixmap;
    // 加载图片
    if (avatarPath.startsWith(":")) {
        // 从资源文件加载
        originalPixmap.load(avatarPath);
    } else {
        // 从文件系统加载
        originalPixmap.load(avatarPath);
    }
    if (originalPixmap.isNull()) {
        qDebug() << "无法加载头像:" << avatarPath;
        // 加载默认头像
        originalPixmap.load("://img/默认头像.svg");
        if (originalPixmap.isNull()) {
            qDebug() << "默认头像也无法加载";
            return;
        }
    }

    QSize labelSize = label->size();
    QPixmap circlePixmap(labelSize);
    circlePixmap.fill(Qt::transparent); // 背景设为透明
    // 4. 创建QPainter，在临时画布上绘制
    QPainter painter(&circlePixmap);
    painter.setRenderHint(QPainter::SmoothPixmapTransform); // 平滑绘制

    // 5. 定义圆形裁剪路径（基于label尺寸，确保是正圆）
    QPainterPath circlePath;
    // 椭圆参数：x=0, y=0（画布左上角），宽=label宽，高=label高（保证圆形与label大小一致）
    circlePath.addEllipse(0, 0, labelSize.width(), labelSize.height());
    painter.setClipPath(circlePath); // 设置裁剪路径（超出圆形的部分不显示）

    // 6. 缩放原图并绘制到圆形区域内
    // 缩放规则：保持宽高比，缩放到刚好能填满label（避免拉伸）
    QPixmap scaledPixmap = originalPixmap.scaled(
        labelSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    // 计算绘制位置（让缩放后的图片居中显示在label里）
    int x = (labelSize.width() - scaledPixmap.width()) / 2;
    int y = (labelSize.height() - scaledPixmap.height()) / 2;
    painter.drawPixmap(x, y, scaledPixmap); // 绘制到圆形裁剪区域内
    label->setPixmap(circlePixmap);
}
void MainWindow::onProfileUpdateResponse(const QJsonObject &userData)
{

    QString avatarPath = userData["avatar"].toString();
    if (!avatarPath.isEmpty()) {
        // 检查文件是否存在（对于资源文件或绝对路径）
        if (avatarPath.startsWith(":") || QFile::exists(avatarPath)) {
            avatar(avatarPath, ui->label_7);
            avatar(avatarPath, ui->label_2);
            m_avatarPath = avatarPath;
            qDebug() << "头像加载成功:" << avatarPath;
        } else {
            // 如果头像路径不存在，使用默认头像
            QString defaultAvatar = "://img/默认头像.svg";
            avatar(defaultAvatar, ui->label_7);
            avatar(avatarPath, ui->label_2);
            m_avatarPath = defaultAvatar;
            qDebug() << "使用默认头像:" << defaultAvatar;
        }
    } else {
        // 如果没有头像路径，使用默认头像
        QString defaultAvatar = "://img/默认头像.svg";
        avatar(defaultAvatar, ui->label_7);
        avatar(avatarPath, ui->label_2);
        m_avatarPath = defaultAvatar;
        qDebug() << "头像路径为空，使用默认头像";
    }
    ui->label_16->setText(userData["username"].toString());
    ui->label_3->setText(QString::number(userData["level"].toInt()));
    ui->label_5->setText(QString::number(userData["age"].toInt()));
    ui->label_6->setText(userData["birthday"].toString());
    ui->label_4->setText(userData["signature"].toString());

    // 设置编辑页面的默认值

    ui->user_nameEdit->setText(userData["username"].toString());
    ui->agelineEdit->setText(QString::number(userData["age"].toInt()));
    ui->birthdaylineEdit->setText(userData["birthday"].toString());
    ui->signatureEdit->setText(userData["signature"].toString());


    // 保存用户ID和账号
    m_userId = userData["user_id"].toInt();
    m_account = userData["account"].toString(); // 或者使用 username
}
void MainWindow::requestFriendList()
{
    // 请求好友列表
    QJsonObject request;
    request["type"] = "get_user_list";
    request["user_id"] = m_userId;

    QJsonDocument doc(request);
    m_client->m_tcpSocket->write(doc.toJson());

}
void MainWindow::onFriendListReceived(const QJsonObject &friendList)
{
    // 处理服务器返回的好友列表
    ui->friendTree->clear();
    ui->friendTree->setColumnCount(1);
    ui->friendTree->setHeaderHidden(true);
    qDebug()<<"联系人列表"<<friendList["groups"];
    // 解析服务器返回的JSON数据并构建好友列表
    if (friendList.contains("groups")) {
        QJsonArray groups = friendList["groups"].toArray();
        for (const QJsonValue &groupValue : groups) {  // 使用 QJsonValue
            QJsonObject group = groupValue.toObject();  // 转换为 QJsonObjec
            QString groupName = group["group_name"].toString();
            int groupId = group["group_id"].toInt();

            QTreeWidgetItem *groupItem = new QTreeWidgetItem(ui->friendTree);
            groupItem->setText(0, groupName);
            groupItem->setData(0, Qt::UserRole, "group"); // 标记为分组
            groupItem->setData(0, Qt::UserRole + 1, groupId); // 分组ID
            groupItem->setData(0, Qt::UserRole + 2, groupName); // 分组名称
            groupItem->setExpanded(true);

            if (group.contains("contacts")) {
                QJsonArray contacts = group["contacts"].toArray();
                for (const QJsonValue &contactValue : contacts) {
                    QJsonObject contact = contactValue.toObject();
                    QString nickname = contact["nickname"].toString();
                    QString account = contact["account"].toString();
                    QString userName = contact["user_name"].toString();
                    QString avatar = contact["avatar"].toString();
                    int contactId = contact["contact_id"].toInt();
                    int level = contact["level"].toInt();
                    QString signature = contact["signature"].toString();

                    QTreeWidgetItem *contactItem = new QTreeWidgetItem(groupItem);
                    contactItem->setText(0, nickname);
                    contactItem->setIcon(0, QIcon(":/icons/avatar.png"));
                    contactItem->setData(0, Qt::UserRole, "contact"); // 标记为联系人
                    contactItem->setData(0, Qt::UserRole + 1, contactId); // 联系人ID
                    contactItem->setData(0, Qt::UserRole + 2, account); // 账号
                    contactItem->setData(0, Qt::UserRole + 3, nickname); // 昵称
                    contactItem->setData(0, Qt::UserRole + 4, userName); // 用户名
                    contactItem->setData(0, Qt::UserRole + 5, avatar); // 头像
                    contactItem->setData(0, Qt::UserRole + 6, level); // 等级
                    contactItem->setData(0, Qt::UserRole + 7, signature); // 签名
                }
            }
        }
    }
}

void MainWindow::on_dataBackButton_clicked()
{

}


void MainWindow::on_comboBox_activated(int index)
{

}

//添加联系人
void MainWindow::on_pushButton_5_clicked()
{
    QString contact_account = ui->lineEdit->text().trimmed();
    QString group_name = ui->lineEdit_2->text().trimmed();

    // 输入校验
    if (contact_account.isEmpty()) {
        QMessageBox::warning(this, "提示", "账号不能为空！");
        return;
    }
    if (group_name.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择分组！");
        return;
    }

    // 通过客户端发送添加好友请求
    QJsonObject addFriendMsg;
    addFriendMsg["type"] = "add_friend";
    addFriendMsg["contact_account"] = contact_account;
    addFriendMsg["group_name"] = group_name;

    QJsonDocument doc(addFriendMsg);
    m_client->m_tcpSocket->write(doc.toJson());


    // ui->stackedWidget->setCurrentWidget(ui->page4);
    ui->lineEdit->clear();

}

void MainWindow::on_friendTree_itemClicked(QTreeWidgetItem *item, int column)
{
    // 过滤根节点（只点击子节点时弹窗）
    if (item->parent() == nullptr) return;  // parent为null是根节点，不处理

    // 获取点击节点的文本（column=0，对应第1列）
    QString targetnickname = item->data(0, Qt::UserRole + 3).toString(); // 昵称
    QString targetAccount = item->data(0, Qt::UserRole + 2).toString();
    // 4. 实例化并显示弹出窗口（单例模式：避免重复创建）
    if (m_anotherWidget == nullptr) {
        // 创建Chat窗口，传入现有的TCP socket
        m_anotherWidget = new Chat(m_client->m_tcpSocket);
        // 连接聊天消息信号
        connect(m_client, &Client::chatMessageReceived,
                m_anotherWidget, &Chat::onChatMessageReceived);

    }

    // 可选：向弹窗传递点击的节点信息（如成员名称）
    m_anotherWidget->setTargetName(targetnickname,targetAccount);

    // 显示弹窗
    m_anotherWidget->show();
    m_anotherWidget->raise();  // 置顶弹窗（避免被遮挡）
}

