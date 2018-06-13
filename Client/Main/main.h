/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                  Copyright (c) <2011> by <Martin Kohn>.                     //
//                                                                             //
//                     This file is part of Bells.                             //
//                                                                             //
//      Bells is free software: you can redistribute it and/or modify it       //
//      under the terms of the GNU General Public License as published by      //
//      the Free Software Foundation, either version 3 of the License, or      //
//      (at your option) any later version.                                    //
//                                                                             //
//      Bells is distributed in the hope that it will be useful, but           //
//      WITHOUT ANY WARRANTY; without even the implied warranty of             //
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//      GNU General Public License for more details.                           //
//                                                                             //
//      You should have received a copy of the GNU General Public License      //
//      along with Bells.  If not, see <http://www.gnu.org/licenses/>.         //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_H
#define MAIN_H

#include <QSettings>
#include <QTcpSocket>

#define SocketWaitTime 200000

class MainWindow ;
class Settings ;
class Help ;
class About ;

extern bool ServerUp ; /**< Set true if the server is running */
extern QTcpSocket *tcpSocket ; /**< Socket handle for client/server tcp link */
extern QSettings *settings ; /**< A pointer to the application configuration data */
extern MainWindow *MainWin ; /**< A pointer to the mainwindow dialog */
extern Settings *SettingsDialog ; /**< A pointer to the Settings dialog */
extern Help *HelpDialog ; /**< A pointer to the Help dialog */
extern About *AboutDialog ; /**< A pointer to the About dialog */

#endif // MAIN_H
