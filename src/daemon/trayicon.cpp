/*
    SnoreNotify is a Notification Framework based on Qt
    Copyright (C) 2013-2014  Patrick von Reth <vonreth@kde.org>

    SnoreNotify is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SnoreNotify is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SnoreNotify.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "trayicon.h"
#include "core/settingsdialog.h"
#include "core/snore.h"
#include "core/snore_p.h"

#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>

#include "core/version.h"

using namespace Snore;

TrayIcon::TrayIcon():
    m_trayIcon(new QSystemTrayIcon(QIcon(":/root/snore.png"))),
    m_app("SnoreNotify Test", Icon(":/root/snore.png")),
    m_alert("Default", Icon(":/root/snore.png"))
{
    m_app.addAlert(m_alert);
    m_app.hints().setValue("tray-icon", m_trayIcon);
}

void TrayIcon::initConextMenu()
{
    m_settings = new SettingsDialog();
    m_trayIcon->setVisible(true);

    m_trayMenu = new QMenu("SnoreNotify");
    QString version = QString("SnoreNotify %1").arg(Version::version());
    if (!Version::revision().isEmpty()) {
        version += QString("-%1").arg(Version::revision());
    }
    m_trayMenu->addAction(version);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("Test Notification", this, SLOT(slotTestNotification()));
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("Settings", this, SLOT(slotSettings()));
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("Exit", qApp, SLOT(quit()));

    m_trayIcon->setContextMenu(m_trayMenu);
}

void TrayIcon::hide()
{
    m_trayIcon->setVisible(false);
}

QSystemTrayIcon *TrayIcon::trayIcon()
{
    return m_trayIcon;
}

void TrayIcon::slotTestNotification()
{

    if (!SnoreCore::instance().aplications().contains(m_app.name())) {
        SnoreCore::instance().registerApplication(m_app);
    }
    Notification noti(m_app, m_alert, "Hello World",
                      "<i>This is Snore</i><br>"
                      "<a href=\"https://github.com/TheOneRing/Snorenotify\">Project Website</a><br>"
                      "1<br>"
                      "2<br>"
                      "3<br>"
                      "4<br>"
                      "5<br>", Icon(":/root/snore.png"));
    noti.addAction(Action(1, "Test Action"));
    SnoreCore::instance().broadcastNotification(noti);

    QTimer *timer = new QTimer(this);
    m_notifications[timer] = noti;
    timer->setSingleShot(true);
    timer->setInterval(noti.timeout() / 2 * 1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(sloutUpdateTestNotification()));
    timer->start();

    //    SnoreCore::instance().deregisterApplication(app);
}

void TrayIcon::sloutUpdateTestNotification()
{
    QTimer *timer = qobject_cast<QTimer *>(sender());
    Notification noti = m_notifications.take(timer);
    Notification update(noti, "Hello World",
                        "<b>This is Snore</b><br>"
                        "<u>This icon is quite a long line of text, isnt it I think it is what do you think? btw the icon should be in color</u><br>"
                        "<a href=\"https://github.com/TheOneRing/Snorenotify\">Project Website</a>",
                        Icon("http://winkde.org/~pvonreth/other/kde-logo.png"));
    SnoreCore::instance().broadcastNotification(update);
    timer->deleteLater();
}

void TrayIcon::slotSettings()
{
    m_settings->show();
}

