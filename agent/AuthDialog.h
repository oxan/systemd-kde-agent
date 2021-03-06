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

#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include "ui_AuthDialog.h"

#include <QSettings>

class AuthDialog : public KDialog, private Ui::AuthDialog
{
    Q_OBJECT
public:
    AuthDialog(const QString & file, QSettings & settings);
    ~AuthDialog();

public slots:
    virtual void accept();
    void askFileDeleted(const QString & file);
    void dialogAccepted();
    void dialogCanceled();

private:
    QString m_passwordFileName;
    QString m_socketLocation;

    void sendResponse(bool result, const QString & password);
};

#endif // AUTHDIALOG_H
