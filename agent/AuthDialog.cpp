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

#include "AuthDialog.h"

#include <QtGui/QPainter>
#include <QSettings>
#include <QStringList>

#include <KDebug>

#include <KUser>
#include <KProcess>

AuthDialog::AuthDialog(const QString & file, QSettings & settings)
        : KDialog(0, Qt::Dialog), m_passwordFileName(file)
{
    setupUi(mainWidget());
    // the dialog needs to be modal to darken the parent window
    setModal(true);
    setButtons(Ok | Cancel);

    settings.beginGroup("Ask");
    QString message = settings.value("Message", "Enter password").toString();
    QString iconName = settings.value("Icon").toString();
    m_socketLocation = settings.value("Socket").toString();
    settings.endGroup();

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
    const int iconSize = icon.size().width();
    // restrict the emblem icon to size 32
    int overlaySize = 32;
    // try to load the action icon
    const QPixmap pixmap = KIconLoader::global()->loadIcon(iconName,
                                                           KIconLoader::NoGroup,
                                                           overlaySize,
                                                           KIconLoader::DefaultState,
                                                           QStringList(),
                                                           0,
                                                           true);
    // if we're able to load the action icon paint it over the
    // key icon.
    if (!pixmap.isNull()) {
        QPoint startPoint;
        // bottom right corner
        startPoint = QPoint(iconSize - overlaySize - 2,
                            iconSize - overlaySize - 2);
        painter.drawPixmap(startPoint, pixmap);
    }

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

void AuthDialog::sendResponse(bool result, const QString & password)
{
    kDebug() << "Sending response to systemd:" << result;

    KProcess * pkexec = new KProcess();
    pkexec->setProgram("/usr/bin/pkexec", QStringList() << "/lib/systemd/systemd-reply-password" << (result ? "1" : "0") << m_socketLocation);
    pkexec->start();
    if (result) {
        pkexec->write(password.toUtf8());
        pkexec->write("\n");
    }
    pkexec->waitForFinished();

    kDebug() << "Sent response to systemd";
    deleteLater();
}

void AuthDialog::askFileDeleted(const QString & file)
{
    if (m_passwordFileName == file) {
        hide();
        deleteLater();
    }
}

void AuthDialog::dialogAccepted()
{
    hide();
    sendResponse(true, lePassword->text().toUtf8());
    deleteLater();
}

void AuthDialog::dialogCanceled()
{
    hide();
    sendResponse(false, NULL);
    deleteLater();
}