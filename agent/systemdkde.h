/*  This file is part of the KDE project
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

#ifndef SYSTEMDKDE_H
#define SYSTEMDKDE_H

#include "AuthDialog.h"

#include <KUniqueApplication>
#include <KDirWatch>

class SystemdKDE : public KUniqueApplication
{
    Q_OBJECT
public:
    SystemdKDE();
    virtual ~SystemdKDE();

private:
    KDirWatch *m_dirWatch;
    AuthDialog *m_dialog;

private slots:
    void created(const QString & path);
};

#endif
