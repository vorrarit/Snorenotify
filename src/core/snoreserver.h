/****************************************************************************************
 * Copyright (c) 2010-2012 Patrick von Reth <patrick.vonreth@gmail.com>                 *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#ifndef SNORESERVER_H
#define SNORESERVER_H

#include "snore_exports.h"
#include "interface.h"

#include <QStringList>

class QSystemTrayIcon;
class QDir;

namespace Snore{
class SNORE_EXPORT SnoreServer:public QObject
{
    Q_OBJECT
public:
	static const QString version();
    static const QString snoreTMP();
    static QHash<QString,SnorePluginInfo*> pluginCache(const QString &pluginPath = QString());
    static QHash<QString,SnorePluginInfo*> updatePluginCache(const QString &pluginPath = QString());

public:
    SnoreServer (QSystemTrayIcon *trayIcon=0 );
    void publicatePlugin ( const SnorePluginInfo *info );


    uint broadcastNotification ( Notification notification );
    void closeNotification ( Notification notification, const NotificationEnums::CloseReasons::closeReasons &reason );
    void notificationActionInvoked ( Notification notification );

    void addApplication ( Application *application );
    void applicationIsInitialized ( Application* application );
    void removeApplication ( const QString& appName );
    const ApplicationsList &aplications() const;

    const QStringList &primaryNotificationBackends() const;
    void setPrimaryNotificationBackend ( const QString &backend );
    const QString &primaryNotificationBackend();
    QSystemTrayIcon *trayIcon();



private:
    static const QDir &pluginDir(const QString &pluginPath);


    static QHash<QString,SnorePluginInfo*> m_pluginCache;
    ApplicationsList m_applications;


    QHash<QString,Notification_Backend*> m_notyfier;
    QHash<QString,Notification_Frontend*> m_frontends;
    QStringList m_primaryNotificationBackends;
    Notification_Backend * m_notificationBackend;
    QHash<QString,SnorePlugin*> m_plugins;

    QSystemTrayIcon *m_trayIcon;


signals:
    void applicationInitialized ( Snore::Application* );
    void applicationRemoved ( Snore::Application* );
    void notify ( Snore::Notification noti );
    void actionInvoked( Snore::Notification );
    void closeNotify ( Snore::Notification );

};

}

#endif // SNORESERVER_H
