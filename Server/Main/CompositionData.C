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

#include "CompositionData.h"

// Each method is used either while parsing the composition file
// OR while ringing the composition
// except for CompositionData::CurrentMethod() which is used during both phases


void CompositionData::AddMethod (void) {
// Used while parsing
  // Add a new empty MethodData object to the MethodList
  // And keep track of it via CurrentMethodData
  CurrentMethodData = new MethodData ;
  MethodList.push_back (*CurrentMethodData) ;
}


MethodData *CompositionData::CurrentMethod (void) {
// Used while parsing AND while ringing
  return CurrentMethodData ;
}

MethodData::MethodData (void ) {
// Used while parsing
  // Allocate space for the first row of place notation
  PnRow = new PlaceRow ;
}

void MethodData::SetNoOfBells (const char*NrBells) {
// Used while parsing
  sscanf (NrBells, "%d", &NoOfBells) ;
}

void MethodData::SetMethodName (const char*Name) {
// Used while parsing
  MethodName = Name ;
  cout << MethodName << endl ;
}

void MethodData::AddPlace (const char*Place) {
// Used while parsing
  int place ;
  // Convert Place to an int
  sscanf (Place, "%d", &place) ;
  // Add to the set of places (PnRow)
  PnRow->insert (place) ;
}

void MethodData::NewPlaceRow (void ) {
  // Allocate space for a new row of place notation
  PnRow = new PlaceRow ;
}

void MethodData::AddPlaceRow (void ) {
// Used while parsing
  // The current place notation row is pushed onto the PlaceRows list
  PlaceRows.push_back (*PnRow) ;
}

void MethodData::AddPlainLE (void ) {
// Used while parsing
  PlainLE = *PnRow ;
}

void MethodData::AddBobLE (void ) {
// Used while parsing
  BobLE = *PnRow ;
}

void MethodData::AddSingleLE (void ) {
// Used while parsing
  SingleLE = *PnRow ;
}

int MethodData::GetNoOfBells (void) {
// Used while ringing
  return NoOfBells ;
}

list<MethodData::PlaceRow> * MethodData::GetPn (void) {
// Used while ringing
  return &PlaceRows ;
}

MethodData::PlaceRow * MethodData::GetPlainLE (void) {
// Used while ringing
  return &PlainLE ;
}

MethodData::PlaceRow * MethodData::GetBobLE (void) {
// Used while ringing
  return &BobLE ;
}

MethodData::PlaceRow * MethodData::GetSingleLE (void) {
// Used while ringing
  return &SingleLE ;
}

void MethodData::ListPn (void) {
// For debugging only
// Not currently used
// Prints out  all the place notation
// to standard output

  int i,j ;
  list<PlaceRow>::iterator it ;
  PlaceRow::iterator jt ;

  it = PlaceRows.begin() ;
  for (i=0; i<PlaceRows.size(); it++, i++) {
	jt = it->begin() ;
	for (j=0; j<it->size(); jt++, j++)
	  cout << *jt << " " ;
	cout << endl ;
  }
  cout << "PlainLE " ;
  jt = PlainLE.begin() ;
  for (j=0; j<PlainLE.size(); jt++, j++)
	cout << *jt << " " ;
  cout << endl ;
  cout << "BobLE " ;
  jt = BobLE.begin() ;
  for (j=0; j<BobLE.size(); jt++, j++)
	cout << *jt << " " ;
  cout << endl ;
  cout << "SingleLE " ;
  jt = SingleLE.begin() ;
  for (j=0; j<SingleLE.size(); jt++, j++)
	cout << *jt << " " ;
  cout << endl ;
}
