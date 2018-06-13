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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QProcess>
#include <QSettings>
#include "ui_SettingsBase.h"


/**
 * @brief Handles the Settings dialog
 *
 */
class Settings : public QDialog, public Ui::SettingsBase {
Q_OBJECT
public:

  /**
   * @brief Settings Constructor
   *
   * @param parent pointer to Parent widget
   * @param f Window flags
   */
  Settings ( QWidget * parent = 0, Qt::WindowFlags f = 0 );

private slots:

  /**
   * @brief Saves the information represented by the dialog
   * @brief to the application's config file
   *
   */
  void Save (void) ;

  /**
   * @brief Sets the minimum value of the InitialNoteLength widget
   * and the minimum value of the MaxNoteLength widget
   *
   * @param Value The minimum value
   */
  void MinNoteLengthValueChanged (int Value) ;

  /**
   * @brief Sets the maximum value of the InitialNoteLength widget
   * and the maximum value of the MinNoteLength widget
   *
   * @param Value The maximum value
   */
  void MaxNoteLengthValueChanged (int Value) ;

  /**
   * @brief Commands the server to set the midi channel
   *
   * @param Value The value of the midi channel
   */
  void SetMidiChannel (int Value) ;

  /**
   * @brief Sets the path of the composition file in the Composition widget
   * @brief Activated by clicking a file browser button
   *
   */
  void SetComposition (void) ;

  /**
   * @brief Sets the path of the server executable in the Server widget
   * @brief Activated by clicking a file browser button
   *
   */
  void SetServer (void) ;

private:

};

#endif
