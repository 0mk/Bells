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

#include <iostream>
#include <sstream>
#include "Bells.h"

using namespace std ;


ServerSocket *ClientLink;


void HandleOptions (int argc, char **argv, string& CompositionPath, int& Port)
{
  int  c;

  string ServerPort ;

  /* Deal with the options */
  for (;;) {
	c = getopt (argc, argv, "m:p:");
	if (c == -1)
	  break;

	  switch(c) {
		case 'm':
		  // Set the composition path
		  CompositionPath = optarg ;
		break;
		case 'p': {
		  // Set the server tcp port number
		  ServerPort = optarg ;
		  // Now parse the ServerPort string to check it is a valid number.
		  std::istringstream InputStream(ServerPort);
		  int Value = 0 ;
		  if (InputStream >> Value) {
			// ... Success!!  Port is a number.
			Port = Value ;
		  }
		  else {
			// ... Failure!!  Port is not a number.
			// Warn the user.
			cout << "Warning: No such port " << ServerPort << endl ;
			cout << "Warning: Setting port to 1500 instead." << endl ;
			// Port was already set to 1500 by default
		  }
		}
		break;
		case '?':
		  // Unexpected option
		  // getopt will write a message to stderr
		  throw "" ;
	  }
  }
}


// Program entry point
int main (int argc, char **argv) {
  // Note: The client is started by the user first.
  // The client then creates and starts this server,
  // passing it the composition path and tcp port number
  // in the command-line arguments.
  // Further client communications are then done via tcp.

  Ringer *R  ; // Used to start and control the ringer thread

  // Set default values for Composition path and tcp port
  string CompositionPath = "Composition" ; // Path of composition file
  int Port = 1500 ; // tcp port used for client communication

  try { // Handle command line options which were set by the client
	HandleOptions (argc, argv, CompositionPath, Port) ;
  }
  catch ( char const *Message ) {
	if (Message != "")
	  cout << Message << endl ;
	exit (1) ;
  }
  catch ( char *Message ) {
	if (Message != "")
	  cout << Message << endl ;
	exit (1) ;
  }

  try {
	// Create the socket for communication with all clients.
	// This is standard server socket protocol and must be done first
	// since the server socket is designed to handle multiple client connections
	// despite the fact we have only one client.
	ServerSocket server ( Port );

	// Create a ringer object to handle the ringing.
	// The ringing will be done in a new thread.
	R = new Ringer (CompositionPath) ;

	// Create a socket for a specific client instance.
	// In principle, this could be repeated for many concurrent clients
	// However,we actually only do this once since we have only one client.
	ClientLink = new ServerSocket ;

	// Accept connection from the client.
	server.accept ( (*ClientLink) );


	// Start the ringer thread
	R->Start () ;

	try { // Handle client commands here
	  while ( true ) {
		string command;

		// Read Client command from socket
		(*ClientLink) >> command;

		// Tell the ringer thread about the client command
		// by invoking the appropriate method of the ringer object.
		if (command == "Ring") {
		  R->Ring () ; // Start ringing the current method
        }
		else if (command == "Abort") {
		  R->Abort () ; // Abort the current method
        }
		else if (command == "Bob") {
		  R->Bob () ; // Do a bob at the next lead-end
        }
		else if (command == "Single") {
		  R->Single () ; // Do a single at the next lead-end
        }
		else if (command == "Plain") {
		  R->Plain () ; // Do a plain at the next lead-end
        }
		else if (command == "Kill") {
		  R->Kill () ; // Exit
		  // throw an exception to exit the program.
		  throw "" ;
		}

		else if (command.substr(0,11) == "InitialData") {
		  // We have an InitialData command.
		  // Extract Note length and midi channel from command string
		  string Data = command.substr(12) ;
		  int pos = Data.find_first_of(' ') ;
		  string NoteLen = Data.substr(0,pos) ;
		  string MidiChan = Data.substr(pos+1) ;
		  // Now parse the NoteLen string to check it is a valid number.
		  std::istringstream InputStream(NoteLen);
		  int Value = 0 ;
		  if (InputStream >> Value) {
			// ... Success!!  value is a number.
			// so tell the ringer thread
			R->SetNoteLength (Value) ; // Set the new note length
		  }
		  else {
			// ... Failure!!  value is not a number.
			// Warn the user and terminate.
			throw "Garbled Note Length!" ;
		  }
		  // Now parse the MidiChan string to check it is a valid number.
		  std::istringstream InputStream2(MidiChan);
		  Value = 0 ;
		  if (InputStream2 >> Value) {
			// ... Success!!  value is a number.
			// so tell the ringer thread
			R->SetMidiChannel (Value) ; // Set the midi channel
		  }
		  else {
			// ... Failure!!  value is not a number.
			// Warn the user and terminate.
			throw "Garbled Midi Channel!" ;
		  }
		} // Finished processing InitialData command

		else if (command.substr(0,2) == "NL") {
		  // We have a NoteLength command.
		  // Extract Note length from command string
		  string NoteLen = command.substr(3) ;
		  // Now parse the NoteLen string to check it is a valid number.
		  std::istringstream InputStream(NoteLen);
		  int Value = 0 ;
		  if (InputStream >> Value) {
			// ... Success!!  value is a number.
			// so tell the ringer thread
			R->SetNoteLength (Value) ; // Set the new note length
		  }
		  else {
			// ... Failure!!  value is not a number.
			// Warn the user and terminate.
			throw "Garbled Note Length!" ;
		  }
		} // Finished processing NoteLength command

		else if (command.substr(0,11) == "MidiChannel") {
		  // We have a MidiChannel command.
		  // Extract midi channel from command string
		  string MidiChan = command.substr(12) ;
		  // Now parse the MidiChan string to check it is a valid number.
		  std::istringstream InputStream(MidiChan);
		  int Value = 0 ;
		  if (InputStream >> Value) {
			// ... Success!!  value is a number.
			// so tell the ringer thread
			R->SetMidiChannel (Value) ; // Set the midi channel
		  }
		  else {
			// ... Failure!!  value is not a number.
			// Warn the user and terminate.
			throw "Garbled Midi Channel!" ;
		  }
		} // Finished processing MidiChannel command

		else {
		  // We got an unexpected command !
		  // Warn the user
		  cout << "Unexpected command received:  " << command << endl ;
		}
	  }
	}
	catch ( SocketException& ) {
	  // Ignore
	}
  }
  catch ( SocketException& e ) {
	// Exception caught
	// Warn the user and terminate
	cout << "Socket exception: " << e.description() << endl ;
  }
  catch ( char *Message ) {
	// Exception caught
	// Warn the user and terminate
	if (Message != "")
	  cout << Message << endl ;
  }
  catch ( char const *Message ) {
	// Exception caught
	// Warn the user and terminate
	if (Message != "")
	  cout << Message << endl ;
  }

  // Wait for ringer thread to finish.
  R->Join () ;

  // Tell the user we are finished.
  cout << "Server terminating" << endl << endl ;

  // That's it.  We now cease to exist.
}
