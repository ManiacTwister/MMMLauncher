/*
 * Desktop.cpp
 *
 * This file is part of INVERITA.
 *
 * INVERITA Personal Backup Software
 * Copyright (C) 2012-2014  Marc Weidler <marc.weidler@web.de>,
 *                          Ulrichstr. 12/1, 71672 Marbach, Germany.
 *                          All rights reserved.
 *
 * INVERITA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * INVERITA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "desktop.h"

#include <QProcessEnvironment>
#include <QIcon>


/*! Constructs a new Desktop object
 */
Desktop::Desktop()
{
}


/*! Destructor
 */
Desktop::~Desktop()
{
}


/* handle environmend dependencies.
   DESKTOP_SESSION identifies the desktop environment:

   Mate 1.2 & 1.4 (Linux Mint 13 and 14)
   -------------------------------------
   DESKTOP_SESSION contains 'default.desktop'
   inverita -determineicontheme="mateconftool-2 --get /desktop/mate/interface/icon_theme"
   ps -ef | grep mateconfd | wc -l   -> 2

   KDE (Kubuntu 12.04)
   -------------------

   QString environment = QProcessEnvironment::systemEnvironment().value("DESKTOP_SESSION");
   QString theme = QIcon::themeName();

   out << "Mate desktop: " << mateDetection.contains("mateconfd") << "\n";
   out << "KDE  desktop: " << mateDetection.contains("xsettings-kde") << "\n";

   Gnome3
   ------

   gsettings get org.gnome.desktop.interface icon-theme


   Xfce4
   -----

   xfconf-query -c xsettings -p /Net/IconThemeName

*/


/*! Execute one command and return it's stdout output
 *
 * \param executable the command to be executed
 * \return the stdout output of the command
 */
QString Desktop::executeCommand(QString executable)
{
    QProcess process;
    QString command;
    QString parameters;

    int firstBlank = executable.indexOf(" ");
    if (firstBlank >= 0) {
        command = executable.left(firstBlank);
        parameters = executable.mid(firstBlank + 1);
    } else {
        command = executable;
        parameters = "";
    }

    process.start(command, parameters.split(" ", QString::SkipEmptyParts));
    if (process.waitForStarted() == false) {
        return "";
    }

    process.waitForFinished();

    QString answer = QString::fromLocal8Bit(process.readAllStandardOutput());
    if (process.exitStatus() != QProcess::NormalExit) {
        return "";
    }

    return answer;
}


/*! Tries to determine the currently running desktop type,
 *  like Mate, KDE or Gnome.
 *
 * \return the determined desktop, or Unknown
 */
Desktop::DesktopType Desktop::determineDesktopType()
{
#ifdef Q_OS_UNIX
    QString processlist = executeCommand("ps -e");

    if (processlist.contains("mateconfd")) {
        return Desktop::Mate14;
    }

    if (processlist.contains("mate-settings")) {
        return Desktop::Mate16;
    }

    if (processlist.contains("gnome-session")) {
        return Desktop::Gnome;
    }

    if (processlist.contains("xsettings-kde")) {
        return Desktop::KDE;
    }

    if (processlist.contains("xfce4-session")) {
        return Desktop::Xfce4;
    }

    if (processlist.contains("lxsession")) {
        return Desktop::Lxde;
    }
#endif

    return Desktop::Unknown;
}


/*! Tries to determine the configured icon theme of the current
 *  desktop enviromnent. Qt 4.8 does not handle this correctly
 *  for modern DEs like Mate and Gnome3.
 *
 * \return the configured icon theme to be used
 */
QString Desktop::determineIconTheme()
{
    QString iconTheme = QIcon::themeName();
    QString tempTheme;

    switch (determineDesktopType()) {
        case Desktop::Mate14:
            tempTheme = executeCommand("mateconftool-2 --get /desktop/mate/interface/icon_theme");
            iconTheme = tempTheme.remove('\n');
            break;

        case Desktop::Mate16:
            tempTheme = executeCommand("gsettings get org.mate.interface icon-theme");
            iconTheme = tempTheme.remove('\n').remove('\'');
            break;

        case Desktop::Xfce4:
            tempTheme = executeCommand("xfconf-query -c xsettings -p /Net/IconThemeName");
            iconTheme = tempTheme.remove('\n');
            break;

        case Desktop::Lxde:
            iconTheme = "gnome";
            break;

        case Desktop::Gnome:
            // no special handling, use standard icon theme
            break;

        case Desktop::KDE:
            // no special handling, use standard icon theme
            break;

        default:
            break;
    }

    return iconTheme;
}
