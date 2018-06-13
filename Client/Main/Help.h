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

#ifndef HELP_H
#define HELP_H

#include <QDialog>
#include "ui_HelpBase.h"


/**
 * @brief Handles the Help dialog
 *
 */
class Help : public QDialog, public Ui::HelpBase {
Q_OBJECT
public:

  /**
   * @brief Help Constructor
   *
   * @param parent pointer to Parent widget
   * @param f Window flags
   */
  Help ( QWidget * parent = 0, Qt::WindowFlags f = 0 );

private slots:

private:

};

#endif
