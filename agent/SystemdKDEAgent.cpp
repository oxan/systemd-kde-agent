/*
    Copyright (C) 2009, 2010 Jaroslav Reznik <jreznik@redhat.com>
    Copyright (C) 2013 Oxan van Leeuwen <oxan@oxanvanleeuwen.nl>

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

#include "SystemdKDEAgent.h"
#include "AuthDialog.h"

#include <QFileInfo>
#include <QSettings>

#include <KDebug>
#include <KWindowSystem>

#include <stdint.h>
#include <time.h>

SystemdKDEAgent::SystemdKDEAgent() : m_dirWatch(0)
{
    setQuitOnLastWindowClosed(false);

    kDebug() << "Creating filesystem watcher";

    m_dirWatch = new KDirWatch(this);
    m_dirWatch->addDir("/run/systemd/ask-password", KDirWatch::WatchFiles);
    connect(m_dirWatch, SIGNAL(created(const QString &)), this, SLOT(created(const QString &)));
}

SystemdKDEAgent::~SystemdKDEAgent()
{
}

void SystemdKDEAgent::created(const QString & path)
{
    QFileInfo pf(path);
    if (pf.baseName() == "ask" && pf.isFile() && !pf.isSymLink())
    {
        kDebug() << "Asking for password due to ask file" << path;

        QSettings ask(path, QSettings::IniFormat);
        ask.beginGroup("Ask");
        qulonglong notAfter = ask.value("NotAfter").toULongLong();
        if(notAfter != 0)
        {
            struct timespec monotime;
            clock_gettime(CLOCK_MONOTONIC, &monotime);
            quint64 time = monotime.tv_sec * 1000000 + monotime.tv_nsec / 1000;

            if(time > ask.value("NotAfter").toULongLong())
            {
                kDebug() << "Ignoring because NotAfter is in the past";
                return;
            }
        }
        ask.endGroup();

        AuthDialog *dialog = new AuthDialog(path, ask);
        connect(m_dirWatch, SIGNAL(deleted(const QString &)), dialog, SLOT(askFileDeleted(const QString &)));
        connect(dialog, SIGNAL(okClicked()), dialog, SLOT(dialogAccepted()));
        connect(dialog, SIGNAL(cancelClicked()), dialog, SLOT(dialogCanceled()));

        dialog->show();
        KWindowSystem::forceActiveWindow(dialog->winId());
    }
}