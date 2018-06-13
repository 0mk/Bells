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

#include "Ringer.h"

Ringer::Ringer (string CompositionPath) {
  try {
	// Create Composition object
	Composition1 = new Composition (CompositionPath) ;

	//Create a Method object
	CurrentMethod = new Method () ;

	// Configure the Method object
	CurrentMethod->Read (Composition1->ParseData->CurrentMethod()) ;

	// We must wait for a client command before ringing starts
	Ringing = false ;

	// But we don't want to die just yet
	Alive = true ;
  }
  // Report any problems
  catch ( char *Message ) {
	if (Message != "")
	  cout << Message << endl ;
  }
}


void Ringer::Ring (void) {
  // Ignore if already ringing
  if (!Ringing) {
	// Start from rounds
	CurrentMethod->Reset () ;
	Ringing = true ;
  }
}

void Ringer::Bob (void) {
  CurrentMethod->Bob () ;
}

void Ringer::Single (void) {
  CurrentMethod->Single () ;
}

void Ringer::Plain (void) {
  CurrentMethod->Plain () ;
}

void Ringer::Abort (void) {
  Ringing = false ;
  CurrentMethod->Abort () ;
}

// Called by the client to terminate
void Ringer::Kill (void) {
  // Set The Ringing flag true to avoid
  // the sleep loop in operator()()
  Ringing = true ;
  Alive = false ;
  // Cause the current method to abort
  CurrentMethod->Abort () ;
}

void Ringer::SetNoteLength (int Value) {
  CurrentMethod->SetNoteLength (Value) ;
}

void Ringer::SetMidiChannel (int Value) {
  CurrentMethod->SetMidiChannel (Value) ;
}

void RingerEntryPoint (void *TheRinger) {
  ((Ringer *)TheRinger)->EntryPoint() ;
}


void Ringer::Start (void) {
  Thread = new thread(RingerEntryPoint, this) ;
}

void Ringer::Join (void) {
  Thread->join() ;
}

//void Ringer::operator()(void) {
void Ringer::EntryPoint (void) {
  while (Alive) {
	while (!Ringing) {
	  //usleep (100000) ;
      this_thread::sleep_for (chrono::milliseconds(100)) ;
	}
	CurrentMethod->Ring () ;
  }
}
