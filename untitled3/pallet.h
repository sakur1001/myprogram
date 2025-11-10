#ifndef PALLET_H
#define PALLET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>
#include <QEvent>

#include <QContextMenuEvent>
#include <QMenu>
#include <QMap>
#include <QList>
#include <QUrl>
#include <QVBoxLayout>
#include <QPainterPath>
#include<QMediaPlayer>
#include <QAudioOutput>
#include<QSlider>
#include <QHBoxLayout>
#include <QWidgetAction>

QT_BEGIN_NAMESPACE

namespace Ui {
class pallet;
}
QT_END_NAMESPACE
namespace Act {
Q_NAMESPACE
enum RoleAct{
    Swing,
    Rotate
};
Q_ENUM_NS(RoleAct)
}
using namespace Act;
// 旋转动画部件（上层）
class RotatableLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
public:
    explicit RotatableLabel(QWidget *parent = nullptr) : QLabel(parent), m_rotation(0) {
        setAlignment(Qt::AlignCenter);
        setStyleSheet("background: transparent;");
    }
    qreal rotation() const { return m_rotation; }
    void setRotation(qreal angle) {
        m_rotation = angle;
        update();
    }
protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        QPainterPath circlePath;
        circlePath.addEllipse(width()/2 - 75, height()/2 - 75, 150, 150);
        painter.setClipPath(circlePath);

        int centerX = width() / 2;
        int centerY = height() / 2;
        painter.translate(centerX, centerY);
        painter.rotate(m_rotation);
        painter.translate(-centerX, -centerY);

        if (!pixmap().isNull()) {
            painter.drawPixmap(0, 0, width(), height(),
                               pixmap().scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }

        QRadialGradient gradient(centerX, centerY, 75);
        gradient.setColorAt(1.0, QColor(244, 246, 187, 255));
        gradient.setColorAt(0.9, QColor(244, 246, 187, 128));
        gradient.setColorAt(0.85, QColor(255, 255, 255, 96));
        gradient.setColorAt(0.7, QColor(255, 255, 255, 64));
        gradient.setColorAt(0.5, QColor(255, 255, 255, 0));
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, width(), height());
    }
private:
    qreal m_rotation;
};

// 帧动画部件（下层）
class FrameAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FrameAnimationWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setStyleSheet("background: transparent;");
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &FrameAnimationWidget::updateFrame);
    }

    // 加载帧动画资源
    void loadFrames(RoleAct act, const QMap<RoleAct, QList<QUrl>>& actionMap) {
        this->actionMap = actionMap;
        currentAct = act;
        frameIndex = 0;
        timer->start(60); // 帧间隔
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        if (actionMap.contains(currentAct) && !actionMap[currentAct].isEmpty()) {
            QUrl frameUrl = actionMap[currentAct][frameIndex];
            QPixmap pix;
            if (pix.load(frameUrl.toLocalFile())) {
                painter.drawPixmap(0, 0, width(), height(), pix.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    }

private slots:
    void updateFrame() {
        if (actionMap.contains(currentAct)) {
            frameIndex = (frameIndex + 1) % actionMap[currentAct].size();
            update();
        }
    }

private:
    QTimer* timer;
    QMap<RoleAct, QList<QUrl>> actionMap;
    RoleAct currentAct;
    int frameIndex = 0;
};

// 帧动画类型枚举




class pallet : public QWidget
{
    Q_OBJECT

public:
    explicit pallet(QWidget *parent = nullptr);
    ~pallet();
    void startMusicAndAnimation();
private slots:

    void onVolumeControlTriggered();//音量
    void onPauseTriggered();     // 处理“暂停/继续”逻辑
    void onNormalSpeedTriggered(); // 处理“正常速度”逻辑
    void onHighSpeedTriggered();   // 处理“高速”逻辑
    void onsuperHighSpeedTriggered();//超速
private:
    Ui::pallet *ui;
    RotatableLabel* rotateLabel;      // 旋转动画（上）
    FrameAnimationWidget* frameWidget;// 帧动画（下）
    QPropertyAnimation* rotateAnim;
    QMenu* menu;
    QMenu *rotateSubMenu;
    QMenu *volumeSubMenu;

    QVBoxLayout* mainLayout;
    QMap<RoleAct, QList<QUrl>> actionMap; // 帧动画资源
    QMediaPlayer*m_player;
    QAudioOutput *m_audioOutput;


    void initResources();     // 加载帧动画资源
    void initUI();            // 初始化界面布局
    void initAnimations();    // 初始化动画
    void initmusic();         //初始化音乐
    void initMenu();          // 初始化菜单

    void contextMenuEvent(QContextMenuEvent *event);
};




#endif // PALLET_H
