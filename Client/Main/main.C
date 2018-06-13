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

#include <QApplication>
#include <QtNetwork>
#include <QTcpSocket>
#include "main.h"
#include "MainWindow.h"
#include "Settings.h"
#include "Help.h"
#include "About.h"


QSettings *settings ;
Settings *SettingsDialog ;
QTcpSocket *tcpSocket ;
bool ServerUp ;
MainWindow *MainWin ;
Help *HelpDialog ;
About *AboutDialog ;

int main(int argc, char **argv) {
  // Set the information needed to create a config directory
  // This directory will contain a file holding the information
  // in our QSettings object
  QCoreApplication::setOrganizationName("BouncyCat");
  QCoreApplication::setOrganizationDomain("BouncyCat.com");
  QCoreApplication::setApplicationName("Bells");

  // Create the server comms link
  tcpSocket = new QTcpSocket() ;

  // Create the config object
  settings = new QSettings ;
  // And put in some default values
  if (!(settings->contains ("Server")))
    settings->setValue ("Server", "Path to Server") ;
  if (!(settings->contains ("Composition")))
    settings->setValue ("Composition", "Path to composition file") ;
  if (!(settings->contains ("Port")))
    settings->setValue ("Port", "1500") ;
  if (!(settings->contains ("MinNoteLength")))
    settings->setValue ("MinNoteLength", 60) ;
  if (!(settings->contains ("InitialNoteLength")))
    settings->setValue ("InitialNoteLength", 80) ;
  if (!(settings->contains ("MaxNoteLength")))
    settings->setValue ("MaxNoteLength", 200) ;
  if (!(settings->contains ("MidiChannel")))
    settings->setValue ("MidiChannel", 0) ;


  // Create the application object
  QApplication app (argc, argv) ;

  // Create the Help dialog (allows the user to view the User Guide)
  HelpDialog = new Help ;
  HelpDialog->Browser->setUrl (QUrl ("qrc:/resources/UserGuide.html")) ;

  // Create the About dialog (allows the user to view information about the application)
  AboutDialog = new About ;

  // Create the Settings dialog (allows the user to configure the application)
  SettingsDialog = new Settings ;

  // Populate its widgets with some default values
  SettingsDialog->Composition->setText(settings->value("Composition").toString()) ;
  SettingsDialog->Server->setText(settings->value("Server").toString()) ;
  SettingsDialog->Port->setValue(settings->value("Port").toInt()) ;
  SettingsDialog->MinNoteLength->setValue(settings->value("MinNoteLength").toInt()) ;
  SettingsDialog->MaxNoteLength->setValue(settings->value("MaxNoteLength").toInt()) ;
  SettingsDialog->InitialNoteLength->setValue(settings->value("InitialNoteLength").toInt()) ;
  SettingsDialog->MidiChannel->setValue(settings->value("MidiChannel").toInt()) ;

  // Create the main window dialog
  MainWin = new MainWindow ;
  // Initialise main window widget values ...
  // Set min and max values of speed slider
  MainWin->UpdateSettings() ;
  // Set actual value of speed slider
  MainWin->NoteLength->setValue(settings->value("InitialNoteLength").toInt()) ;


  // Show Main Window
  MainWin->show();
  
  // Start the application event loop
  return app.exec();
  // Cease all existence
}
