/*  This file is part of the KDE project
    Copyright (C) 2019 Jaroslav Reznik <jreznik@redhat.com>

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

#include <KCmdLineArgs>
#include <KAboutData>
#include <KLocale>

#include "systemdkde.h"

int main(int argc, char *argv[])
{
    KAboutData aboutData("systemd-kde", "systemd-kde-authentication-agent-1", ki18n("Systemd-KDE"), "0.1",
                         ki18n("systemd-KDE"), KAboutData::License_GPL,
                         ki18n("(c) 2010 Red Hat, Inc."));
    aboutData.addAuthor(ki18n("Jaroslav Reznik"), ki18n("Maintainer"), "jreznik@redhat.com");

    KCmdLineArgs::init(argc, argv, &aboutData);

    if (!SystemdKDE::start()) {
        qWarning("systemdKDE is already running!\n");
        return 0;
    }

    SystemdKDE agent;
    agent.exec();
}
