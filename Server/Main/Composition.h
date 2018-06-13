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

#ifndef __Compostion__
#define __Compostion__

#include <fstream>
#include "CompositionParser.h"
#include "ReadPn.l.h"

using namespace std ;

/**
 * @brief This class is used to control the lexing and parsing of the composition file.
 * @brief After creating a lexical analyser and ParseData object, it will construct and then call the parser, leaving the parsed information
 in the ParseData object.
 *
 */
class Composition {
public:

/**
 * @brief Constructor
 *
 * @param FileName The full path of the composition file.
 *
 */
  Composition (string FileName) ;

  CompositionParser *Parser  ; /**< pointer to the composition parser */
  yyFlexLexer *Lexer ;         /**< pointer to the lexical analyser */
  CompositionData *ParseData ; /**< pointer to the parsed information */
} ;

#endif
