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

#include <iostream>
#include <QtWidgets>
#include <QtNetwork>
#include <QAction>
#include <QToolBar>
#include <QThread>
#include "MainWindow.h"
#include "Settings.h"
#include "main.h"
#include "Help.h"
#include "About.h"

using namespace std ;



MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags f) : QMainWindow(parent, f) {
  // Set up the main window widgets
  setupUi(this);

  // Create an object to apply text highlighting to the console window
  highlighter = new BellHighlighter (Console) ;
  (void)highlighter ; // Stop the compiler complaining

  // Create an object to handle the server process
  Server = new QProcess(this);

  // Initialise some flags
  ServerUp = false ; // Server not running yet
  Die = false ; // We shouldn't terminate yet
  Restart = false ; // We shouldn't restart the server yet
  ConsoleTextLength = 0 ; // The console is empty

  // Set the quit button icon
  actionQuit->setIcon (QIcon (":/resources/Quit.svg")) ;

  // This is for a Method/Call changes radio-button group
  // This will not be visible in this release
  // since call-changes are not implemented yet
  ModeGroup = new QActionGroup(this);
  ModeGroup->addAction(actionMethod);
  ModeGroup->addAction(actionCall_Changes);
  actionMethod->setChecked(true);

  // Create the help menu
  HelpMenu = new QMenu (this) ;
  ActionManual = HelpMenu->addAction ("User Manual") ;
  connect (ActionManual, SIGNAL(triggered()), this, SLOT(UserManual())) ;
  ActionAbout = HelpMenu->addAction ("About") ;
  connect (ActionAbout, SIGNAL(triggered()), this, SLOT(About())) ;

  ActionHelp = new QAction (this) ;
  ActionHelp->setToolTip ("Help") ;
  ActionHelp->setText ("") ;
  ActionHelp->setIcon (QIcon (":/resources/Help.svg")) ;
  toolBar->addAction (ActionHelp) ;
  connect (ActionHelp, SIGNAL(triggered()), this, SLOT(Help())) ;

  ActionSettings = new QAction (this) ;
  ActionSettings->setToolTip ("Settings") ;
  ActionSettings->setText ("") ;
  ActionSettings->setIcon (QIcon (":/resources/Settings.svg")) ;
  toolBar->addAction (ActionSettings) ;
  connect (ActionSettings, SIGNAL(triggered()), this, SLOT(Settings())) ;


  // Connect signals to slots

  // Do the button action signals first
  connect (actionRing, SIGNAL(triggered()), this, SLOT(Ring())) ;
  connect (actionBob, SIGNAL(triggered()), this, SLOT(Bob())) ;
  connect (actionSingle, SIGNAL(triggered()), this, SLOT(Single())) ;
  connect (actionPlain, SIGNAL(triggered()), this, SLOT(Plain())) ;
  connect (actionAbort, SIGNAL(triggered()), this, SLOT(Abort())) ;
  connect (actionQuit, SIGNAL(triggered()), this, SLOT(Quit())) ;
  connect (actionReStartServer, SIGNAL(triggered()), this, SLOT(RestartServer())) ;

  // Now do the speed slider signals
  connect(NoteLength, SIGNAL(valueChanged(int)), this, SLOT(SetNoteLength(int)));

  // Now do the Server process signals
  connect(Server, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadServerStandardOutput()));
  connect(Server, SIGNAL(readyReadStandardError()), this, SLOT(ReadServerStandardError()));
  connect(Server, SIGNAL(started()), this, SLOT(ServerStarted()));
  connect(Server, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ServerFinished(int, QProcess::ExitStatus)));

  // Now do the tcp socket signals
  connect(tcpSocket, SIGNAL(connected()), this, SLOT(Connected()));
  connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(ReadServerSocket()));
  connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ServerError(QAbstractSocket::SocketError)));

  // Don't constrain the size of the console
  Console->setSizePolicy (QSizePolicy::Ignored, QSizePolicy::Ignored) ;
  // User can't write to console
  Console->setReadOnly (true) ;
}

MainWindow::~MainWindow (void) {
  delete ModeGroup ;
  delete Server ;
}

void MainWindow::Help(void)
{
  HelpMenu->exec(QCursor::pos()) ;
}

void MainWindow::UserManual (void) {
  // Display the User Manual
  HelpDialog->exec () ;
}

void MainWindow::About (void) {
  // Display the About dialog
  AboutDialog->exec () ;
}


void MainWindow::Settings (void) {
  SettingsDialog->exec () ;
}


void MainWindow::UpdateSettings (void) {
  NoteLength->setMinimum(settings->value("MinNoteLength").toInt()) ;
  NoteLength->setMaximum(settings->value("MaxNoteLength").toInt()) ;
}


BellHighlighter::BellHighlighter (QTextEdit *TextEdit): QSyntaxHighlighter (TextEdit) {
}

// This highlights all occurences of the "pattern" text in the console
// Used to create a 'blue line'
void BellHighlighter::highlightBlock (const QString &text) {
  QTextCharFormat BellFormat;
  BellFormat.setFontWeight(QFont::Bold);
  BellFormat.setForeground(Qt::blue);
  QString pattern = "02";

  QRegExp expression(pattern);
  int index = text.indexOf(expression);
  while (index >= 0) {
    int length = expression.matchedLength();
    setFormat(index, length, BellFormat);
    index = text.indexOf(expression, index + length);
  }
}



// Print standard output of server to console
void MainWindow::ReadServerStandardOutput (void) {
  QByteArray data = Server->readAllStandardOutput () ;
  int DataLength = data.size() ;

  Console->insertPlainText (QString (data)) ;
  ConsoleTextLength += DataLength ;

  //move the cursor to the end of the text
  Console->moveCursor(QTextCursor::End);

  Console->ensureCursorVisible () ;
}

// Print standard error of server to console
void MainWindow::ReadServerStandardError (void) {
  QByteArray data = Server->readAllStandardError () ;
  int DataLength = data.size() ;

  Console->insertPlainText (QString (data)) ;
  ConsoleTextLength += DataLength ;

  //move the cursor to the end of the text
  Console->moveCursor(QTextCursor::End);

  Console->ensureCursorVisible () ;
}


void MainWindow::ReadServerSocket (void) {
  QTextStream in(tcpSocket);
  QString Message ;

  in >> Message ;

  if (Message == "Plain") {
	// A lead-end has occured
	// Default to a plain for next lead-end
    actionPlain->setChecked (true) ;
    actionBob->setChecked (false) ;
    actionSingle->setChecked (false) ;
  }
}



void MainWindow::ServerError (QAbstractSocket::SocketError socketError) {
 switch (socketError) {
   case QAbstractSocket::RemoteHostClosedError:
	 break;
   case QAbstractSocket::HostNotFoundError:
	 QMessageBox::information(this, tr("Error"),
                                  tr("No host was found. Please check the "
                                     "host name and port settings."));
	 break;
   case QAbstractSocket::ConnectionRefusedError:
     if (ConnectionRetries > 100) {
	   QMessageBox::information(this, tr("Error"),
                                  tr("The connection was refused. "
                                     "Make sure the server is running, "
                                     "and check that the host name and port "
                                     "settings are correct."));
		}
		else {
		  // Retry the connection 
		  ConnectionRetries++ ;
          QThread::usleep (10000) ;
          tcpSocket->abort();
		  tcpSocket->connectToHost("localhost", settings->value("Port").toInt()) ;
		  tcpSocket->waitForConnected(SocketWaitTime) ;
		}
	   break;
     default:
	   QMessageBox::information(this, tr("Error"),
                                  tr("The following error occurred: %1.")
                                  .arg(tcpSocket->errorString()));
     }
}


void MainWindow::Bob (void) {
  actionBob->setChecked (true) ;
  actionSingle->setChecked (false) ;
  actionPlain->setChecked (false) ;
  if (tcpSocket->waitForConnected(SocketWaitTime))
	tcpSocket->write("Bob");
}

void MainWindow::Single (void) {
  actionSingle->setChecked (true) ;
  actionBob->setChecked (false) ;
  actionPlain->setChecked (false) ;
  if (tcpSocket->waitForConnected(SocketWaitTime))
        tcpSocket->write("Single");
}

void MainWindow::Plain (void) {
  actionPlain->setChecked (true) ;
  actionBob->setChecked (false) ;
  actionSingle->setChecked (false) ;
  if (tcpSocket->waitForConnected(SocketWaitTime))
        tcpSocket->write("Plain");
}

void MainWindow::Ring (void) {
  if (tcpSocket->waitForConnected(SocketWaitTime))
	tcpSocket->write("Ring");
}


void MainWindow::SetNoteLength (int Value) {
  QByteArray Number ;
  Number.setNum (Value) ;
  if (ServerUp)
    if (tcpSocket->waitForConnected(SocketWaitTime)) {
      tcpSocket->write("NL ");
      tcpSocket->write(Number);
    }
}

void MainWindow::SetInitialData (int NoteLen, int MidiChan) {
  QByteArray Number ;
  if (ServerUp)
    if (tcpSocket->waitForConnected(SocketWaitTime)) {
      tcpSocket->write("InitialData ");
      Number.setNum (NoteLen) ;
      tcpSocket->write(Number);
      tcpSocket->write(" ");
      Number.setNum (MidiChan) ;
      tcpSocket->write(Number);
    }
}

void MainWindow::Abort (void) {
  if (tcpSocket->waitForConnected(SocketWaitTime))
	tcpSocket->write("Abort");
}

void MainWindow::Kill (void) {
  if (tcpSocket->waitForConnected(SocketWaitTime))
	tcpSocket->write("Kill");
}



void MainWindow::ServerStarted (void) {
  // Default to plain-lead
  actionPlain->setChecked (true) ;
  actionBob->setChecked (false) ;
  actionSingle->setChecked (false) ;

  // Set up the tcp socket
  ConnectionRetries = 0 ;
  tcpSocket->abort();
  tcpSocket->connectToHost("localhost", settings->value("Port").toInt()) ;
  tcpSocket->waitForConnected(SocketWaitTime) ;
}


void MainWindow::Connected (void) {
  // Debug to troubleshoot faulty comms
  // Shouldn't be an issue.
  cout << "Connection Retries: " << ConnectionRetries << endl ;

  ServerUp = true ;

  // Send initial data to server
  SetInitialData (NoteLength->value(),  settings->value("MidiChannel").toInt()) ;
}


void MainWindow::RestartServer (void) {
  // We should not terminate
  Die = false ;
  // The server should be restarted
  Restart = true ;
  // Kill the server if it is running
  // In this case ServerFinished() will start a new server
  // and will check the Die and Restart flags set above
  if (Server->state() == QProcess::Running)
	Kill () ;
  else
	// otherwise just start a new server here
	StartServer () ;
}

void MainWindow::StartServer (void) {
  QString ServerPath = settings->value("Server").toString() ;
  // Check for valid Server Path
  if (QFile::exists (ServerPath)) {
    QString ServerPort = settings->value("Port").toString() ;
    QString CompositionPath = settings->value("Composition").toString() ;
	// Check for valid Composition Path
    if (QFile::exists (CompositionPath)) {
	  // If the NEW server dies, ServerFinished() should log it
	  // Unless it is killed by the StartServer button being pressed
      Restart = false ;
	  // Set up the server command line
      QString ServerCommand = ServerPath ;
      ServerCommand += " -m " ;
      ServerCommand += CompositionPath ;
      ServerCommand += " -p " ;
      ServerCommand += ServerPort ;
	  // Start the new server process
      Server->start(ServerCommand);
    }
    else
      QMessageBox::information(this, tr("Error"), tr("Invalid Composition path"));
  }
  else
	QMessageBox::information(this, tr("Error"), tr("Invalid Server path"));
}


void MainWindow::Quit (void) {
  // We should terminate
  Die = true ;
  // The server should not be restarted
  Restart = false ;
  // Kill the server if it is running
  // In this case ServerFinished() will NOT start a new server
  // and will check the Die and Restart flags set above
  if (Server->state() == QProcess::Running)
	Kill () ;
  else
	// Otherwise just exit immediately.
	Exit () ;
}

void MainWindow::Exit (void) {
  if (Die)
	qApp->quit () ;
}

void MainWindow::ServerFinished ( int , QProcess::ExitStatus ) {
  if (Die)
    qApp->quit () ;
  else if (Restart)
    StartServer () ;
  else
	// This flag combination will only happen it the server process died outside our control
    ServerDied () ;
}

void MainWindow::ServerDied (void) {
  ConsoleMessage (tr("Server died unexpectedly!\n")) ;
}

void MainWindow::ConsoleMessage (QString Message) {
  Console->insertPlainText (Message) ;
  Console->moveCursor(QTextCursor::End);
  Console->ensureCursorVisible () ;
}
