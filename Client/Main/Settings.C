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

#include <QtGui>
#include <QtNetwork>
#include <QFileDialog>
#include "Settings.h"
#include "MainWindow.h"
#include "main.h"

Settings::Settings ( QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f) {
  // Set up the dialog widgets
  setupUi (this) ;
  // Set the icon for the composition browser button
  CompositionBrowser->setIcon (QIcon(":/resources/Load.svg")) ;
  // Set the icon for the server browser button
  ServerBrowser->setIcon (QIcon(":/resources/Load.svg")) ;

  // Connect signals to slots
  // Do the OK button first
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(Save())) ;
  // Now the MidiChannel spin box
  connect(MidiChannel, SIGNAL(valueChanged(int)), this, SLOT(SetMidiChannel(int)));
  // Now the MinNoteLength spin box
  connect(MinNoteLength, SIGNAL(valueChanged(int)), this, SLOT(MinNoteLengthValueChanged(int)));
  // Now the MaxNoteLength spin box
  connect(MaxNoteLength, SIGNAL(valueChanged(int)), this, SLOT(MaxNoteLengthValueChanged(int)));
  // Now the CompositionBrowser button
  connect(CompositionBrowser, SIGNAL(clicked(bool)), this, SLOT(SetComposition()));
  // Now the ServerBrowser button
  connect(ServerBrowser, SIGNAL(clicked(bool)), this, SLOT(SetServer()));
}


void Settings::Save (void) {
  // Save all the widget values to the config file
  // via the QSettings object
  settings->setValue ("Server", Server->text()) ;
  settings->setValue ("Composition", Composition->text()) ;
  settings->setValue ("Port", Port->text()) ;
  settings->setValue ("MinNoteLength", MinNoteLength->text()) ;
  settings->setValue ("InitialNoteLength", InitialNoteLength->text()) ;
  settings->setValue ("MaxNoteLength", MaxNoteLength->text()) ;
  settings->setValue ("MidiChannel", MidiChannel->text()) ;
  // Update any Main Window widgets that depend on config settings
  MainWin->UpdateSettings () ;
}


void Settings::MinNoteLengthValueChanged (int Value) {
  InitialNoteLength->setMinimum (Value) ;
  MaxNoteLength->setMinimum (Value) ;
}

void Settings::MaxNoteLengthValueChanged (int Value) {
  InitialNoteLength->setMaximum (Value) ;
  MinNoteLength->setMaximum (Value) ;
}


void Settings::SetMidiChannel (int Value) {
  QByteArray Number ;
  Number.setNum (Value) ;
  if (ServerUp)
	// Communicate the Midi Channel to the server
    if (tcpSocket->waitForConnected(SocketWaitTime)) {
      tcpSocket->write("MidiChannel ");
      tcpSocket->write(Number);
    }
}


void Settings::SetComposition (void) {
  QString HomeDir = getenv("HOME") ;
  QString Result = QFileDialog::getOpenFileName (this, tr("Set Composition"), HomeDir) ;
  if (!(Result.isNull()))
    Composition->setText (Result) ;
}

void Settings::SetServer (void) {
  QString HomeDir = getenv("HOME") ;
  QString Result = QFileDialog::getOpenFileName (this, tr("Set Server"), HomeDir) ;
  if (!(Result.isNull()))
    Server->setText (Result) ;
}
