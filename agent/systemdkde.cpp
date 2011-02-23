/*  This file is part of the KDE project
    Copyright (C) 2010 Jaroslav Reznik <jreznik@redhat.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#include "systemdkde.h"
#include "AuthDialog.h"

#include <KDebug>
#include <QFileInfo>
#include <QSettings>

SystemdKDE::SystemdKDE() : m_dirWatch(0)
{
    setQuitOnLastWindowClosed(false);

    kDebug() << "Setting watch";

    m_dirWatch = new KDirWatch(this);
    m_dirWatch->addDir("/home/jreznik/test/ask-password", KDirWatch::WatchFiles);
    connect(m_dirWatch, SIGNAL(created(const QString &)), this, SLOT(created(const QString &)));
}

SystemdKDE::~SystemdKDE()
{
}

void SystemdKDE::created(const QString & path)
{
    QFileInfo pf(path);
    if (pf.baseName() == "ask" && pf.isFile() && !pf.isSymLink())
    {
        kDebug() << "Ask for password";

        QSettings ask(path, QSettings::IniFormat);
        ask.beginGroup("Ask");
        QString message = ask.value("Message", "Enter password").toString();
        ask.endGroup();

        AuthDialog *authDialog = new AuthDialog(path);
        connect(m_dirWatch, SIGNAL(deleted(const QString &)), authDialog, SLOT(finishDialog(const QString &)));

        authDialog->show();
    }
}
