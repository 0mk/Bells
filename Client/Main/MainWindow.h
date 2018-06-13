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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QTcpSocket>
#include <QProcess>
#include <QSettings>
#include <QFile>
#include <QSyntaxHighlighter>
#include "ui_MainWindowBase.h"


/**
 * @brief Applies a syntax highlighter to the console text
 *
 */
class BellHighlighter : public QSyntaxHighlighter {
public:

  /**
   * @brief The constructor for BellHighlighter
   *
   * @param Console is passed here
   */
  BellHighlighter (QTextEdit * Console) ;

  /**
   * @brief The syntax highlighting algorithm
   *
   * @param Text The text to highlight
   */
  void highlightBlock (const QString &Text) ;

} ;


/**
 * @brief Handles the main dialog window of Bells
 *
 */
class MainWindow : public QMainWindow, public Ui::MainWindowBase {
Q_OBJECT
public:

  /**
   * @brief Main Window Constructor
   *
   * @param parent pointer to Parent widget
   * @param f Window flags
   */
  MainWindow( QWidget * parent = 0, Qt::WindowFlags f = 0 );

  /**
   * @brief Main Window Destructor
   *
   */
  ~MainWindow( );

  /**
   * @brief Applies any configuration settings that affect widgets in the main window
   *
   */
  void UpdateSettings (void) ;

private slots:

  /**
   * @brief Commands the server to set the note length
   * @brief Issued whenever the slider is moved
   *
   * @param NoteLength
   */
  void SetNoteLength (int NoteLength) ;

  /**
   * @brief Commands the server to set the initial note length and midi channel
   *
   * @param NoteLength
   * @param MidiChannel
   */
  void SetInitialData (int NoteLength, int MidiChannel) ;

  /**
   * @brief Commands the server to start ringing
   *
   */
  void Ring (void) ;

  /**
   * @brief Commands the server to perform a bob at next leadend
   *
   */
  void Bob (void) ;

  /**
   * @brief Commands the server to perform a single at next leadend
   *
   */
  void Single (void) ;

  /**
   * @brief Commands the server to perform a plain at next leadend
   *
   */
  void Plain (void) ;

  /**
   * @brief Commands the server to abort ringing
   *
   */
  void Abort (void) ;

  /**
   * @brief Commands the server to terminate
   *
   */
  void Kill (void) ;

  /**
   * @brief Starts up the server process
   * @brief Must only be called if no server is running
   *
   */
  void StartServer (void) ;

  /**
   * @brief Kills any existing server process
   * @brief Then starts up a new server process
   *
   */
  void RestartServer (void) ;

  /**
   * @brief Called once the server process has started
   * @brief Sets up a tcp link to the server
   * @brief Sends initial data to server
   *
   */
  void ServerStarted (void) ;

  /**
   * @brief Called once the server process has finished
   * @brief Either quits the application or starts a new server
   * @brief depending on the state of the Die and Restart flags.
   *
   * @param exitCode   Ignored
   * @param status     Ignored
   */
  void ServerFinished (int exitCode, QProcess::ExitStatus status) ;

  /**
   * @brief Kills any existing server process
   * @brief Then quits the application
   *
   */
  void Quit (void) ;

  /**
   * @brief Quits the application
   *
   */
  void Exit (void) ;

  /**
   * @brief Called if the server process has finished unexpectedly
   * @brief Prints a message to that effect to the console
   *
   */
  void ServerDied (void) ;

  /**
   * @brief Prints a message to the console
   *
   * @param Message The message to print
   */
  void ConsoleMessage (QString Message) ;

  /**
   * @brief Brings up the Help Menu
   *
   */
  void Help (void) ;

  /**
   * @brief Brings up the User Manual
   *
   */
  void UserManual (void) ;

  /**
   * @brief Brings up the About Dialog
   *
   */
  void About (void) ;

  /**
   * @brief Brings up the Settings dialog
   *
   */
  void Settings (void) ;

  /**
   * @brief Called when the tcp socket has initialized
   * @brief Sends the initial note length and midi channel
   * to the server. \n
   * Sets the ServerUp flag true.
   *
   */
  void Connected (void) ;

  /**
   * @brief Processes tcp messages from the server
   * @brief Used to detect when the leadend occurs
   *
   */
  void ReadServerSocket (void) ;

  /**
   * @brief Processes standard output text from the server
   * @brief Prints it to the console
   *
   */
  void ReadServerStandardOutput (void) ;

  /**
   * @brief Processes standard error text from the server
   * @brief Prints it to the console
   *
   */
  void ReadServerStandardError (void) ;

  /**
   * @brief Called if an error occured in the tcp socket
   *
   * @param socketError The type of error that occurred
   */
  void ServerError(QAbstractSocket::SocketError socketError);

private:
  QAction *ActionSettings ;
  QAction *ActionHelp ;
  QAction *ActionManual ;
  QAction *ActionAbout ;

  QMenu *HelpMenu ;

  bool Die ; /**< Set true if the application should terminate */

  bool Restart ; /**< Set true if the server should restart */
  
  int ConnectionRetries ; /**< The number of server socket connections attempted */


  int ConsoleTextLength ; /**< Holds the length of text in the console */

  QProcess *Server; /**< Handle for the server process */

  BellHighlighter *highlighter ;

  QActionGroup *ModeGroup ;  /**< Used to create an action group to group
                               the Method and Call_Changes actions which are
                               mutually exclusive. Call changes are not implemented
                               in this release so these actions are not realised. */
};


#endif
