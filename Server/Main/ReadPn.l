/*///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////*/

Digit	[0-9]
Letter	[a-zA-Z]

%{
#include <iostream>
#include "CompositionData.h"
#include "ReadPn.tab.hh"
#include "LexFuncs.h"
typedef yy::CompositionParser::token token ;
int yywrap (void) ;
%}

%%

"Composition"	{ Debug ("CompositionHeader") ; return (token::CompositionHeader) ; }
"Method"		{ Debug ("MethodHeader") ; return (token::MethodHeader) ; }
"PlaceNotation"	{ Debug ("PlaceNotationHeader") ; return (token::PlaceNotationHeader) ; }
"PlainLE"		{ Debug ("PlainLEHeader") ; return (token::PlainLEHeader) ; }
"BobLE"			{ Debug ("BobLEHeader") ; return (token::BobLEHeader) ; }
"SingleLE"		{ Debug ("SingleLEHeader") ; return (token::SingleLEHeader) ; }
"X"				{ Debug ("Cross") ; return (token::Cross) ; }
"."				{ Debug ("Dot") ; return (token::Dot) ; }
{Letter}+		{ Debug ("Word") ; return (token::Word) ; }
{Digit}+		{ Debug ("Number") ; return (token::Number) ; }
[\t\n]+			/* eat whitespace */
.				/* ignore anything else */

%%

using namespace std ;

int yywrap (void) {
  return 1 ;
}

int yyFlexLexer::yywrap (void) {
  return 1 ;
}


void Debug (const char *Text) {
  //cerr <<  Text << endl ;
}
