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

#include "AuthDialog.h"

#include <QtGui/QPainter>
#include <QSettings>
#include <QStringList>

#include <KDebug>

#include <KUser>
#include <KProcess>

AuthDialog::AuthDialog(const QString & file)
        : KDialog(0, Qt::Dialog), m_passwordFileName(file)
{
    setupUi(mainWidget());
    // the dialog needs to be modal to darken the parent window
    setModal(true);
    setButtons(Ok | Cancel);

    QSettings ask(m_passwordFileName, QSettings::IniFormat);
    ask.beginGroup("Ask");
    QString message = ask.value("Message", "Enter password").toString();
    m_socketLocation = ask.value("Socket").toString();
    ask.endGroup();

    if (message.isEmpty()) {
        kWarning() << "Could not get action message for action.";
        lblHeader->setText("<h3>" + ki18n("No action message provided").toString() + "</h3>");
    } else {
        kDebug() << "Message of action: " << message;
        lblHeader->setText("<h3>" + message + "</h3>");
        setCaption(message);
    }

    // loads the standard key icon
    QPixmap icon = KIconLoader::global()->loadIcon("dialog-password",
                                                    KIconLoader::NoGroup,
                                                    KIconLoader::SizeHuge,
                                                    KIconLoader::DefaultState);
    // create a painter to paint the action icon over the key icon
    QPainter painter(&icon);

    setWindowIcon(icon);
    lblPixmap->setPixmap(icon);

    lePassword->setFocus();
}

AuthDialog::~AuthDialog()
{
}

void AuthDialog::accept()
{
    // Do nothing, do not close the dialog. This is needed so that the dialog stays
    lePassword->setEnabled(false);
    return;
}

void AuthDialog::closeDialog()
{
    hide();
    deleteLater();
}

void AuthDialog::askFileDeleted(const QString & file)
{
    if (m_passwordFileName == file)
        closeDialog();
}

void AuthDialog::dialogAccepted()
{
    kDebug() << "Password dialog accepted";
    hide();

    // Inspired by the GNOME agent
    KProcess * pkexec = new KProcess();
    pkexec->setProgram("/usr/bin/pkexec", QStringList() << "/lib/systemd/systemd-reply-password" << "1" << m_socketLocation);
    pkexec->start();
    pkexec->write(lePassword->text().toUtf8());
    pkexec->write("\n");
    pkexec->waitForFinished();

    kDebug() << "Send password to systemd";
    deleteLater();
}

void AuthDialog::dialogCanceled()
{
    kDebug() << "Password dialog canceled";
    closeDialog();
}