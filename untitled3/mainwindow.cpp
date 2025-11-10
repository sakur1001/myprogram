#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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



void  MainWindow::InitALL(const QSqlDatabase &db,int userId,QString account)
{
    m_db = db;
    m_userId = userId;
    m_account=account;
    InitData();
    listfriend();

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

void MainWindow::on_submitButton_clicked()
{
    // int userId = m_userId;
    int level=0;
    auto avatar=m_avatarPath ;
    int age = ui->agelineEdit->text().trimmed().toInt(); // 转换为int类型后再绑定
    auto birthday=ui->birthdaylineEdit->text().trimmed();
    auto signature=ui->signatureEdit->toPlainText().trimmed();
    QSqlQuery query(m_db);
    query.prepare("UPDATE user_profile SET avatar=:avatar,level=:level, signature=:signature, age=:age, birthday=:birthday WHERE user_id=:userId");
    query.bindValue(":avatar", avatar);
    query.bindValue(":signature",signature);
    query.bindValue(":level",level);
    query.bindValue(":age", age);
    query.bindValue(":birthday", birthday);
    query.bindValue(":userId", m_userId);

    if (query.exec()) {
        QMessageBox::information(this, "提示", "资料更新成功！");
        InitData();
    }else {
        QMessageBox::warning(this, "提示", "资料更新失败：" + query.lastError().text());
    }
}
void MainWindow::avatar(QString avatarPath,QLabel* label)
{
    QPixmap originalPixmap(avatarPath);
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
void MainWindow::InitData()
{
    QSqlQuery insertQuery(m_db);
    insertQuery.prepare(R"(
        INSERT INTO user_profile (user_id, avatar, level, signature, age, birthday)
        VALUES (:userId, '', 0, '', 0, '2000-01-01')
        ON DUPLICATE KEY UPDATE user_id=user_id  -- 存在则不执行，不存在则插入
    )");
    insertQuery.bindValue(":userId", m_userId);
    insertQuery.exec();

    QSqlQuery query(m_db);
    query.prepare("SELECT avatar,level, signature, age, birthday FROM user_profile WHERE user_id = :userId");
    query.bindValue(":userId", m_userId);

    if (query.exec()&& query.next()) {

        // 1. 头像（假设数据库存储的是图片路径）
        QString avatarPath = query.value("avatar").toString();
        if (avatarPath.isEmpty()) {
            avatarPath = "://background/cs.png";
        }
        if (!avatarPath.isEmpty()) {
            avatar(avatarPath,ui->label_2);
            // 将处理好的圆形头像设置到label
            avatar(avatarPath,ui->label_7);
        }

        // 2. 等级
        int level = query.value("level").toInt();
        ui->label_3->setText(QString("LV%1").arg(level));

        // 3. 签名
        auto signature = query.value("signature").toString();
        ui->label_4->setText(signature.isEmpty() ? "暂无签名" :signature);
        ui->signatureEdit->setText(signature.isEmpty() ? "暂无签名" :signature);
        // 4. 年龄
        int age = query.value("age").toInt();
        ui->label_5->setText(QString::number(age));
        ui->agelineEdit->setText(QString::number(age));
        // 5. 生日
        auto birthday = query.value("birthday").toString();
        ui->label_6->setText(birthday.isEmpty() ? "2000-01-01" : birthday);
        ui->birthdaylineEdit->setText(birthday.isEmpty() ? "2000-01-01" : birthday);
        qDebug() <<m_userId;

    } else {
        qDebug() << "查询用户资料失败："<< query.lastError().text();
    }
}

void MainWindow::listfriend()
{
    qDebug() <<m_userId;
    QTreeWidget *friendTree = ui->friendTree;
    friendTree->clear(); // 清空原有内容
    friendTree->setColumnCount(1);
    friendTree->setHeaderHidden(true);

    QMap<int, QTreeWidgetItem*> groupItemMap; // 存储“分组ID-分组项”的映射

    // 步骤1：查询当前用户的所有分组
    int userId = m_userId;
    QSqlQuery insertGroupQuery;
    QString insertSql = QString("INSERT INTO contact_groups (group_name, user_id) VALUES ('我的好友', %1)  "
                                "ON DUPLICATE KEY UPDATE user_id=user_id  -- 存在则不执行，不存在则插入")
                            .arg(userId);
    insertGroupQuery.exec(insertSql);


    QSqlQuery groupQuery;
    groupQuery.prepare("SELECT group_id, group_name FROM contact_groups WHERE user_id = :userId");
    groupQuery.bindValue(":userId", userId);
    bool Group = false;
    if (groupQuery.exec()) {
        while (groupQuery.next()) {


            int groupId = groupQuery.value("group_id").toInt();
             qDebug() << "当前遍历分组：" << groupId ; // 日志1：确认循环是否执行
            if (!Group){
                QSqlQuery insertContactQuery;
                qDebug() << "触发插入管理员，groupId：" << groupId;
                QString insertContactSql = QString("INSERT INTO contacts (nickname, user_id, group_id) VALUES ('管理员', %1, %2)"
                                                   "ON DUPLICATE KEY UPDATE user_id=user_id ,group_id=group_id -- 存在则不执行，不存在则插入")
                                               .arg(userId).arg(groupId);
                if (!insertContactQuery.exec(insertContactSql)) {
                    qDebug() << "插入管理员失败！错误信息：" << insertContactQuery.lastError().text();
                    qDebug() << "执行的SQL语句：" << insertContactSql;
                }
                Group=true;
            }
            QString groupName = groupQuery.value("group_name").toString();
            QTreeWidgetItem *groupItem = new QTreeWidgetItem(friendTree);
            ui->comboBox->addItem(groupName);
            groupItem->setText(0, groupName);
            groupItem->setExpanded(true); // 默认展开分组
            groupItemMap.insert(groupId, groupItem);
        }
    }





    // 步骤2：查询当前用户的所有联系人
    QSqlQuery contactQuery;
    QString contactSql = QString("SELECT contact_id, nickname, group_id FROM contacts WHERE user_id = %1")
                             .arg(m_userId);
    if (contactQuery.exec(contactSql)) {
        while (contactQuery.next()) {
            QString nickname = contactQuery.value("nickname").toString();
            int groupId = contactQuery.value("group_id").toInt();

            // 确定联系人所属分组（无分组则创建“无分组”项）
            QTreeWidgetItem *parentItem = groupItemMap.value(groupId, nullptr);
            if (!parentItem) {
                parentItem = new QTreeWidgetItem(friendTree);
                parentItem->setText(0, "无分组");
                groupItemMap.insert(-1, parentItem);
            }

            // 创建联系人节点并设置信息
            QTreeWidgetItem *contactItem = new QTreeWidgetItem(parentItem);
            contactItem->setText(0, nickname);
            contactItem->setIcon(0, QIcon(":/icons/avatar.png")); // 联系人头像
            contactItem->setData(0, Qt::UserRole, contactQuery.value("contact_id").toInt()); // 存储联系人ID，用于删除
        }
    }


}

void MainWindow::on_dataBackButton_clicked()
{

}


void MainWindow::on_comboBox_activated(int index)
{

}


void MainWindow::on_pushButton_5_clicked()
{
    QString nickname=ui->lineEdit->text().trimmed();
    QString group_name=ui->comboBox->currentText();

    // 输入校验
    if (nickname.isEmpty()) {
        QMessageBox::warning(this, "注册提示", "账号不能为空！");
        return;
    }
    if (group_name.isEmpty()) {
        QMessageBox::warning(this, "注册提示", "密码不能为空！");
        return;
    }
    int groupId = getGroupIdByGroupName(group_name);
    if (groupId == -1) {
        QMessageBox::warning(this, "提示", "未找到对应的分组，请先创建分组！");
        return;
    }

    // 插入数据库
    QSqlQuery queryadd(m_db);
    queryadd.prepare("INSERT INTO contacts (nickname, group_id, user_id) VALUES (:nickname, :group_id, :user_id)");
    queryadd.bindValue(":nickname", nickname);
    queryadd.bindValue(":group_id", groupId);
    queryadd.bindValue(":user_id", m_userId);

    if (queryadd.exec()) {
        listfriend();
        QMessageBox::information(this, "提示", "添加成功！");

        ui->stackedWidget->setCurrentWidget(ui->page4);
        ui->lineEdit->clear();
    }
}
int MainWindow::getGroupIdByGroupName(const QString &groupName)
{
    QSqlQuery query(m_db);
    // SQL：根据分组名称和用户ID，查询唯一的group_id
    query.prepare("SELECT group_id FROM contact_groups WHERE group_name = :groupName AND user_id = :userId");
    query.bindValue(":groupName", groupName);
    query.bindValue(":userId", m_userId);

    if (query.exec() && query.next()) {
        return query.value("group_id").toInt(); // 找到则返回group_id
    }
    return -1; // 未找到则返回-1（需后续处理）
}

void MainWindow::on_friendTree_itemClicked(QTreeWidgetItem *item, int column)
{
    // 过滤根节点（只点击子节点时弹窗）
    if (item->parent() == nullptr) return;  // parent为null是根节点，不处理

    // 获取点击节点的文本（column=0，对应第1列）
    QString itemText = item->text(column);
    QString Myname = m_account;
    // 4. 实例化并显示弹出窗口（单例模式：避免重复创建）
    if (m_anotherWidget == nullptr) {
        m_anotherWidget = new Client();  // this作为父窗口，关闭主窗口时自动销毁弹窗
        // 可选：设置弹窗为非模态（show()）或模态（exec()）
        // 模态：exec() → 弹窗打开时主窗口不可操作；非模态：show() → 可同时操作两个窗口
    }

    // 可选：向弹窗传递点击的节点信息（如成员名称）
    m_anotherWidget->setTargetName(itemText,Myname);

    // 显示弹窗
    m_anotherWidget->show();
    m_anotherWidget->raise();  // 置顶弹窗（避免被遮挡）
}

