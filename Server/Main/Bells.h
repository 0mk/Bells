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

#ifndef __Bells__
#define __Bells__


#include "CompositionData.h"
#include "ReadPn.l.h"

#include "ServerSocket.h"
#include "SocketException.h"

#include "Ringer.h"

/**
 * @brief Handles command-line options, setting the CompositionPath and Port number.
 *
 * @param argc Number of command-line arguments
 * @param argv Command-line argument array
 * @param CompositionPath Full path of the composition file
 * @param Port Tcp port number for client communication
 */
extern void HandleOptions (int argc, char **argv, string& CompositionPath, int& Port) ;

/**
 * @brief The Server program entry point.
 *
 * @param argc Number of command-line arguments
 * @param argv Command-line argument array
 * @return int The error code of the program or zero for success.
 */
extern int main (int argc, char** argv) ;

extern ServerSocket *ClientLink ; /**< A pointer to the tcp socket for client communication */

#endif
