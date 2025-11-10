#include "widget.h"

#include <QApplication>
#include<QSystemTrayIcon>
#include<QIcon>
#include <QObject>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    // 系统托盘设置
    QSystemTrayIcon sysTray(QIcon("://background/yy.png"), &w);
    QMenu trayMenu;
    auto showAct = new QAction("显示窗口", &sysTray);
    auto exitAct = new QAction("退出", &sysTray);

    QObject::connect(showAct, &QAction::triggered, [&](){ w.setVisible(true); });
    QObject::connect(exitAct, &QAction::triggered, [&](){ QApplication::quit(); });

    trayMenu.addAction(showAct);
    trayMenu.addAction(exitAct);
    sysTray.setContextMenu(&trayMenu);
    sysTray.show();

    w.show();
    return a.exec();
}
