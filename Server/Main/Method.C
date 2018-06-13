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

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include "Method.h"
#include "Bells.h"
#include "tinythread.h"

using namespace std ;


void Method::PlayNote (int Value) {
  // Play a note on the midi device
  MidiPortal->PlayNote (Value, NoteLength) ;
}

Method::Method (void) {
  Tenor = 40 ;
  NoOfRounds = 4 ;
  NoteLength = 100 ;
  Aborted = false ;
  Stroke = handstroke ;
  MidiPortal = new MidiOutput("Bells", "Output", 2) ;
  MidiPortal->SetTempo (40000) ;
}

void Method::SetMidiChannel (int Value) {
  MidiPortal->SetChannel (Value) ;
}

void Method::SetNoteLength (int Value) {
  NoteLength = Value ;
  SetDelays () ;
}

void Method::SetDelays (void) {
  // Set the handstroke and backstroke pause lengths
  HandstrokeDelay = 0 ;
  BackstrokeDelay = NoteLength*2 ;
}


int Method::Major (int Number) {
  // Convert a bell number (modulo 7) to a chromatic note (modulo 12) on the major scale
  switch (Number) {
	case 0: return 0 ; break ;
	case 1: return 2 ; break ; // T
	case 2: return 4 ; break ; // T
	case 3: return 5 ; break ; // S
	case 4: return 7 ; break ; // T
	case 5: return 9 ; break ; // T
	case 6: return 11 ; break ;// T
	//   7:        12 ;       ;// S
  }
}

void Method::Read (MethodData *MethodData) {
  int i ;

  NrBells = MethodData->GetNoOfBells() ;
  SetDelays () ;

  // Allocate space for sequences
  Rounds = new vector<int>(NrBells) ;
  MidiSequence = new vector<int>(NrBells) ;
  BellSequence1 = new vector<int>(NrBells) ;
  BellSequence2 = new vector<int>(NrBells) ;

  // Set the place notation
  Pn = MethodData->GetPn () ;
  PlainLE = MethodData->GetPlainLE () ;
  BobLE = MethodData->GetBobLE () ;
  SingleLE = MethodData->GetSingleLE () ;

  // Prepare for ringing from rounds
  Reset () ;
}

void Method::Bob (void) {
  Leadend = bob ;
}

void Method::Single (void) {
  Leadend = single ;
}

void Method::Plain (void) {
  Leadend = plain ;
}

void Method::Abort (void) {
  Aborted = true ;
}

void Method::Reset (void) {
  // Prepare for ringing from rounds
  Aborted = false ;
  Stroke = handstroke ;
  ThisSequence = &BellSequence1 ;
  NextSequence = &BellSequence2 ;
  for (int i=0; i< NrBells; i++) {
	(*MidiSequence)[NrBells-i-1] = Tenor + 12*(i/7) + Major(i%7) ;
	(*BellSequence1)[i] = (*Rounds)[i] = i ;
  }
}

void Method::Ring (void) {
  string Plain ;
  Plain = "Plain" ;
  try {
	// Ring a couple of rounds to start.
	PlaySequence () ;
	PlaySequence () ;

	// Now ring the method proper until it comes round.
	for (;;) {
	  // Apply the place notation one row at a time to take us to the half-lead.
	  for ( PnFit=Pn->begin(); PnFit != Pn->end(); PnFit++ ) {
		GetNextSequence (PnFit->begin(), PnFit->end()) ;
		PlaySequence () ;
	  }
	  // Methods are symmetric about the half-lead so apply the place notation in reverse
	  // to take us to the lead end.
	  for ( PnRit= ++(Pn->rbegin()); PnRit != Pn->rend(); PnRit++ ) {
		GetNextSequence (PnRit->begin(), PnRit->end()) ;
		PlaySequence () ;
	  }
	  // Now apply the correct place notation for the lead end.
	  switch (Leadend) {
		case plain:
		  GetNextSequence (PlainLE->begin(), PlainLE->end()) ;
		  break ;
		case bob:
		  GetNextSequence (BobLE->begin(), BobLE->end()) ;
		  break ;
		case single:
		  GetNextSequence (SingleLE->begin(), SingleLE->end()) ;
		  break ;
	  }
	  PlaySequence () ;

	  // Default to plain for the next lead end.
	  // This can be changed by the client asynchronously.
	  Leadend = plain ;

	  // Tell Client about the lead end
	  (*ClientLink) << Plain ;
	  
	  // If we have rounds, we're done.
	  if (IsRounds())
		break ;
	}
	// Play a few final rounds to finish.
	RepeatSequence (NoOfRounds) ;
  }
  catch (char *Message) {
	if (Message != "")
	  cout << Message << endl ;
  }
  catch (char const *Message) {
	if (Message != "")
	  cout << Message << endl ;
  }
}

bool Method::IsRounds (void) {
  for (int i=0; i< NrBells; i++)
	if ( (**ThisSequence)[i] != (*Rounds)[i] )
	  return false ;
  return true ;
}

void Method::RepeatSequence (int NoOfTimes) {
  for (int i=0; i<NoOfTimes; i++)
	PlaySequence () ;
}

void Method::PlaySequence (void) {
  int i ;

  // Abort the method if we must
  if (Aborted)
	throw ("Method Aborted") ;

  // Set the correct delay for the stroke
  if (Stroke==handstroke)
	Delay = HandstrokeDelay ;
  else
	Delay = BackstrokeDelay ;

  // Print the sequence to the standard output
  for (i=0; i< (*ThisSequence)->size(); i++) {
	cout << setw(2) << right << setfill('0') <<  (**ThisSequence)[i]+1 << " " ;
  }
  cout << endl ;

  // Play the sequence on the midi device
  for (i=0; i< (*ThisSequence)->size(); i++) {
	PlayNote((*MidiSequence)[(**ThisSequence)[i]]) ;
  }

  // Pause appropriately
  //usleep (Delay) ;
  this_thread::sleep_for (chrono::milliseconds(Delay)) ;

  // Swap stroke for next time
  if (Stroke==handstroke)
	Stroke = backstroke ;
  else
	Stroke = handstroke ;
}

void Method::GetNextSequence (MethodData::PlaceRow::iterator Start, MethodData::PlaceRow::iterator End) {

  vector<int> **p ;
  int i ;

  // Apply the place notation to ThisSequence to produce NextSequence
  for (i=0; i< NrBells; i++) 
	if (find (Start, End, i+1) != End) {
	  // i is in the set of places.
	  // Make places at position i
	  (**NextSequence)[i] = (**ThisSequence)[i] ;
	}
	else {
	  // i is not in the set of places.
	  // swap the ith bell with the (i+1)th bell
	  (**NextSequence)[i] = (**ThisSequence)[i+1] ;
	  (**NextSequence)[i+1] = (**ThisSequence)[i] ;
	  i++ ;
	}


  // Now Make the new sequence the current one
  // by swapping ThisSequence and NextSequence
  p = ThisSequence ;
  ThisSequence = NextSequence ;
  NextSequence = p ;
}
