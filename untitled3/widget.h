#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QEvent>
#include<QMouseEvent>
#include<QDebug>
#include<QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QToolButton>
#include <QPointer>

#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    int userId;
    QSqlDatabase db;
    MainWindow*ppage2=NULL;//保存页面二地址

private slots:
    void on_login_clicked();

    void on_radioButton_clicked();

    void on_close_toolButton_clicked();

    void on_back_pushButton_clicked();

    void on_regback_pushButton_clicked();
    void on_regist_clicked();

    void on_confirm_pushButton_clicked();
    void initui();
    void regcjeck();
    void initsql();
private:
    Ui::Widget *ui;


    bool checkAccountExists(const QString &account); // 检查账号是否已存在
};

class DragWidgetFilter : public QObject{        //拖拽过滤器
public:
    DragWidgetFilter(QObject*parent)
        :QObject(parent){}
    bool eventFilter(QObject*object,QEvent*event)
    {
        auto w=dynamic_cast<QWidget*>( object);
        if(!w)
        {
            return false;
            // return QObject::eventFilter(object, event);
        }
        if(event->type()==QEvent::MouseButtonPress)
        {
            auto ev=dynamic_cast<QMouseEvent*>(event);
            if(!ev)
            {
                return false;
                // return QObject::eventFilter(object, event);
            }
            pos=ev->pos();
            return QObject::eventFilter(object, event);
        }
        else if(event->type()==QEvent::MouseMove)
        {

            auto ev=dynamic_cast<QMouseEvent*>(event);
            if(!ev)
            {
                return false;
                // return QObject::eventFilter(object, event);
            }
            if(ev->buttons()&Qt::MouseButton::LeftButton)
            {
                w->move(ev->globalPosition().toPoint()-pos);
            }

        }
        return QObject::eventFilter(object, event);
    }
private:
    QPoint pos;
};
class MenuLeaveFilter : public QObject
{
    Q_OBJECT
private:
    QPointer<QToolButton> m_toolBtn; // 关联的工具按钮
    QPointer<QMenu> m_menu;           // 关联的菜单
public:
    explicit MenuLeaveFilter(QToolButton *toolBtn, QMenu *menu, QObject *parent = nullptr)
        : QObject(parent), m_toolBtn(toolBtn), m_menu(menu) {}

    bool eventFilter(QObject *obj, QEvent *event) override
    {
        // 只处理鼠标移动事件
        if (event->type() == QEvent::MouseMove) {
            QPoint globalMousePos = QCursor::pos(); // 获取鼠标全局坐标

            // 检查鼠标是否在ToolButton区域内
            bool inToolBtn = false;
            if (m_toolBtn) {
                QPoint localPos = m_toolBtn->mapFromGlobal(globalMousePos);
                inToolBtn = m_toolBtn->rect().contains(localPos);
            }

            // 检查鼠标是否在Menu区域内
            bool inMenu = false;
            if (m_menu && m_menu->isVisible()) {
                QPoint localPos = m_menu->mapFromGlobal(globalMousePos);
                inMenu = m_menu->rect().contains(localPos);
            }

            // 若鼠标既不在ToolButton也不在Menu上，关闭菜单
            if (!inToolBtn && !inMenu) {
                m_menu->close();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }
};
class MenuFilter : public QObject
{
    Q_OBJECT
public:
    explicit MenuFilter(QObject *parent = nullptr) : QObject(parent) {}

    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::Enter) {
            QToolButton *toolBtn = qobject_cast<QToolButton*>(obj);
            if (toolBtn && toolBtn->objectName() == "toolButton_2") {
                QMenu *menu = toolBtn->menu();
                if (menu) {
                    QPoint popupPos = toolBtn->mapToGlobal(QPoint(0, toolBtn->height()));
                    menu->move(popupPos); // 定位菜单
                    menu->show(); // 非阻塞弹出

                    // 关键修改2：给菜单安装离开过滤器（监听自身鼠标离开）
                    menu->installEventFilter(new MenuLeaveFilter(toolBtn,menu)); // 父对象设为菜单，自动析构
                    return true;
                }
            }
                // 可继续添加更多控件的判断...
        }
        // 其他事件交给父类处理
        return QObject::eventFilter(obj, event);
    }
};

#endif // WIDGET_H
