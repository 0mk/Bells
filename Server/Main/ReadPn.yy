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

%skeleton "lalr1.cc"
%defines
%define "parser_class_name" {CompositionParser}
%lex-param {yyFlexLexer* Lexer}
%parse-param {yyFlexLexer* Lexer}
%parse-param {CompositionData* ParseData}
%token Word
%token Cross
%token Dot
%token Number
%token CompositionHeader
%token MethodHeader
%token PlaceNotationHeader
%token PlainLEHeader
%token BobLEHeader
%token SingleLEHeader
%start Composition

%{
#include "CompositionData.h"
#include <FlexLexer.h>
int yylex (int*, yyFlexLexer*) ;
%}

%%

Composition		: CompositionHeader Sections
				;

Sections		: Section
				| Sections Section
				;

Section			: Method
				;

Method			: MethodHeader MethodName {
					ParseData->AddMethod() ;
					ParseData->CurrentMethod()->SetMethodName (Lexer->YYText()) ;
				  }
				  NoOfBells {
					ParseData->CurrentMethod()->SetNoOfBells (Lexer->YYText()) ;
				  }
				  PlaceNotation {
				  }
				;

MethodName		: Word
				;

NoOfBells		: Number
				;

PlaceNotation	: PlaceNotationHeader PlaceRows PlainLE BobLE SingleLE
				;

PlaceRows		: PlaceRow
				| PlaceRows PlaceRow
				;

PlainLE			: PlainLEHeader PlaceList Dot {
					ParseData->CurrentMethod()->AddPlainLE () ;
					ParseData->CurrentMethod()->NewPlaceRow () ;
				  }
				;

BobLE			: BobLEHeader PlaceList Dot {
					ParseData->CurrentMethod()->AddBobLE () ;
					ParseData->CurrentMethod()->NewPlaceRow () ;
				  }
				;

SingleLE			: SingleLEHeader PlaceList Dot {
					ParseData->CurrentMethod()->AddSingleLE () ;
				  }
				;

PlaceRow		: Cross {
					ParseData->CurrentMethod()->AddPlaceRow () ;
					ParseData->CurrentMethod()->NewPlaceRow () ;
				  }
				| PlaceList Dot {
					ParseData->CurrentMethod()->AddPlaceRow () ;
					ParseData->CurrentMethod()->NewPlaceRow () ;
				  }
				;

PlaceList		: Place {
					ParseData->CurrentMethod()->AddPlace (Lexer->YYText()) ;
				  }
				| PlaceList Place {
					ParseData->CurrentMethod()->AddPlace (Lexer->YYText()) ;
				  }
				;

Place			: Number
				;
%%

int yylex (int* Val, yyFlexLexer* Lexer)  {
  return Lexer->yylex() ;
}

namespace yy   {
  void CompositionParser::error (const std::string& msg) {
	throw ("Bad Composition - Server exiting...") ;
  }
}
