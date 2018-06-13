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

#include "Composition.h"

Composition::Composition (string CompositionPath) {
  // Check the composition path is valid
  ifstream *CompositionFileStream = new ifstream (CompositionPath.c_str()) ;
  if (CompositionFileStream->ios::fail())
	throw ("Cannot open composition file") ;

  // Create lexer, ParseData object and composition parser
  Lexer = new yyFlexLexer(CompositionFileStream) ;
  ParseData = new CompositionData ;
  Parser = new CompositionParser (Lexer, ParseData) ;
  // Parse the composition file
  Parser->parse() ;
  // The parsed results are now in the ParseData object
}
