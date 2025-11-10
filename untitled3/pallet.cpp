#include "pallet.h"
#include "ui_pallet.h"
#include "widget.h"
#include<QSystemTrayIcon>
pallet::pallet(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pallet)
    , rotateAnim(new QPropertyAnimation)
    , menu(new QMenu(this))

{
    ui->setupUi(this);



    this->installEventFilter(new DragWidgetFilter(this)); // 支持拖动
    // 初始化流程
    initResources();
    initUI();
    initAnimations();
    initMenu();
    initmusic();


}

pallet::~pallet()
{
    delete ui;
}


void pallet::initResources()
{
    auto addFrames = [this](RoleAct act, QString path, int count) {
        QList<QUrl> frames;
        char buf[260];
        for (int i = 0; i < count; ++i) {
            sprintf_s(buf, path.toStdString().c_str(), i);
            frames.append(QUrl::fromLocalFile(buf));
        }
        actionMap.insert(act, frames);
    };
    addFrames(RoleAct::Swing, "://img/swing/swing_%d.png", 26); // 帧动画资源
}

// 初始化垂直布局（旋转在上，帧动画在下）
void pallet::initUI()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);//窗口透明
    setStyleSheet("background: transparent;");
    // 旋转动画部件（上）
    rotateLabel = new RotatableLabel(this);
    rotateLabel->setPixmap(QPixmap(":/background/cs.png"));
    rotateLabel->setGeometry(0, 0, 150, 150); // 位置(x=0, y=0)，宽高200
    rotateLabel->setScaledContents(true);

    // 帧动画部件（下）
    frameWidget = new FrameAnimationWidget(this);
    frameWidget->setGeometry(-17, 110, 180, 180);


}

// 初始化旋转动画
void pallet::initAnimations()
{
    rotateAnim->setTargetObject(rotateLabel);
    rotateAnim->setPropertyName("rotation");
    rotateAnim->setDuration(5000);
    rotateAnim->setStartValue(0);
    rotateAnim->setEndValue(360);
    rotateAnim->setLoopCount(-1);
}

void pallet::initmusic()
{
    m_player=new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput;
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.2); // 音量设为最大（0.0~1.0）
    m_player->setSource(QUrl("qrc:/music/小阿七-从前说.mp3"));
    m_player->setLoops(QMediaPlayer::Infinite); // 传入Infinite表示无限循环
}

// 菜单控制
void pallet::initMenu()
{
    menu = new QMenu(this);


    // ========== 新增：创建“控制旋转”子菜单 ==========
    rotateSubMenu = new QMenu("控制旋转", this);
    QAction *pauseAction = rotateSubMenu->addAction("暂停", this, &pallet::onPauseTriggered);
    QAction *normalSpeedAction = rotateSubMenu->addAction("正常速度", this, &pallet::onNormalSpeedTriggered);
    QAction *highSpeedAction = rotateSubMenu->addAction("高速", this, &pallet::onHighSpeedTriggered);
    QAction *superhighSpeedAction = rotateSubMenu->addAction("超速", this, &pallet::onsuperHighSpeedTriggered);
    menu->addMenu(rotateSubMenu); // 将子菜单加入主菜单
    // === 新增：音量调节子菜单 ===



    menu->addAction("隐藏", this, [this](){ setVisible(false); });

}
void pallet::onPauseTriggered()
{
    if (rotateAnim->state() == QAbstractAnimation::Running) {
        rotateAnim->pause();
    } else if (rotateAnim->state() == QAbstractAnimation::Paused) {
        rotateAnim->resume();
    }
}

// 处理“正常速度”
void pallet::onNormalSpeedTriggered()
{
    rotateAnim->setDuration(5000); // 恢复默认速度（正常速度）
    if (rotateAnim->state() == QAbstractAnimation::Paused) {
        rotateAnim->resume();
    }
}

// 处理“高速”
void pallet::onHighSpeedTriggered()
{
    rotateAnim->setDuration(1000); // 速度加倍（可根据需求调整数值）
    if (rotateAnim->state() == QAbstractAnimation::Paused) {
        rotateAnim->resume();
    }
}

void::pallet::onsuperHighSpeedTriggered()
{
    rotateAnim->setDuration(100); // 速度加倍（可根据需求调整数值）
    if (rotateAnim->state() == QAbstractAnimation::Paused) {
        rotateAnim->resume();
    }
}



void pallet::onVolumeControlTriggered()
{
    volumeSubMenu = new QMenu("音量调节", this);
    QAction* volumeAct = new QAction("音量", menu);
    volumeAct->setMenu(volumeSubMenu);


    // 1. 创建承载滑块和标签的容器部件
    QWidget* volumeWidget = new QWidget(volumeSubMenu);
    QHBoxLayout* volumeLayout = new QHBoxLayout(volumeWidget);
    volumeLayout->setContentsMargins(0, 0, 0, 0);

    // 2. 定义并初始化音量滑块（仅一次定义）
    QSlider* volumeSlider = new QSlider(Qt::Horizontal, volumeWidget);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(m_audioOutput->volume() * 100);

    // 3. 定义并初始化音量标签
    QLabel* volumeLabel = new QLabel(QString::number(m_audioOutput->volume() * 100) + "%", volumeWidget);

    // 4. 将滑块和标签添加到容器布局
    volumeLayout->addWidget(volumeSlider);
    volumeLayout->addWidget(volumeLabel);

    // 5. 用QWidgetAction包装后添加到子菜单
    QWidgetAction* volumeWidgetAction = new QWidgetAction(volumeSubMenu);
    volumeWidgetAction->setDefaultWidget(volumeWidget);
    volumeSubMenu->addAction(volumeWidgetAction);

    // 6. 滑块值变化时更新音量和标签
    connect(volumeSlider, &QSlider::valueChanged, this, [this, volumeLabel](int value) {
        qreal volume = value / 100.0;
        m_audioOutput->setVolume(volume);
        volumeLabel->setText(QString::number(value) + "%");
    });

    menu->addAction(volumeAct);
}

void pallet::contextMenuEvent(QContextMenuEvent *event)
{
    menu->popup(QCursor::pos());
}



void pallet::startMusicAndAnimation()
{
    rotateAnim->start(); // 启动旋转动画
    frameWidget->loadFrames(RoleAct::Swing, actionMap); // 加载动画帧
    m_player->play(); // 播放音乐
    onVolumeControlTriggered(); // 音量控制触发逻辑
}
