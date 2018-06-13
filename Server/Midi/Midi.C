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
#include "Midi.h"
#include "../Main/Debug.h"
#include "../Main/tinythread.h"

using namespace tthread ;




void MidiOutput::SetTempo (int Value) {
  Tempo = Value ;
}

bool MidiOutput::OpenOutputFile (string FilePath) {
  if (Recording)
    return false ;

  OutputFile.open (FilePath.c_str()) ;
  if (OutputFile.is_open())
    Recording = true ;
  else
    Recording = false ;

  return Recording ;
}

void MidiOutput::CloseOutputFile (void) {
  if (Recording) {
    OutputFile.close () ;
    Recording = false ;
  }
}

void MidiOutput::RecordWait (int Time) {
  if (Recording) {
    OutputFile << "Wait " << Time << endl ;
  }
}


void MidiOutput::Record (vector<unsigned char> *MidiMessage) {
  if (Recording) {
    OutputFile << "MidiMessage" ;
    for (int i=0; i<MidiMessage->size(); i++)
      OutputFile << " " << (int)((*MidiMessage)[i]) ;
    OutputFile << endl ;
  }
}

void MidiOutput::NoteOn (int bus, unsigned int channel, unsigned int Value, unsigned int Velocity) {
  unsigned char Channel ;

  SetBus (bus) ;

  //cout << "MidiOutput::NoteOn " << channel << " " << Value << endl ;

  if (channel <= NoOfChannels)
	Channel = (unsigned char)(channel-1) ;
  else
	// Use 0 as default
	Channel = 0 ;


  // Note On: 144, Value, Velocity
  if ((Value >= 0)&&(Value < 128)) {
	(*MidiMessage3)[0] = (unsigned char)(144 + Channel) ;
	(*MidiMessage3)[1] = (unsigned char)Value ;
	(*MidiMessage3)[2] = (unsigned char)Velocity ;
	MidiOut->sendMessage( MidiMessage3 );
    Record (MidiMessage3) ;
  }
}


void MidiOutput::NoteOff (int bus, unsigned int channel, unsigned int Value, unsigned int Velocity) {
  unsigned char Channel ;

  //cout << "MidiOutput::NoteOff " << channel << " " << Value << endl ;

  SetBus (bus) ;

  if (channel <= NoOfChannels)
	Channel = (unsigned char)(channel-1) ;
  else
	// Use 0 as default
	Channel = 0 ;

  // Note Off: 128, Value, Velocity
  if ((Value >= 0)&&(Value < 128)) {
	(*MidiMessage3)[0] = (unsigned char)(128 + Channel) ;
	(*MidiMessage3)[1] = (unsigned char)Value ;
	(*MidiMessage3)[2] = (unsigned char)Velocity ;
	MidiOut->sendMessage( MidiMessage3 );
    Record (MidiMessage3) ;
  }
}

void MidiOutput::NotesOff (int bus, int channel) {
  Event (bus, 0xb0, channel, 123, 0) ;
}

void MidiOutput::NotesOff (void) {
  dout << "All Notes Off" ;

  for (int bus=0; bus <16; bus++) {
	for (int channel=1; channel <=16; channel++)
	  Event (bus, 0xb0, channel, 123, 0) ;
  }
}


void MidiOutput::Event (int bus, unsigned int EventType, unsigned int channel, unsigned int Value) {
  unsigned char Channel ;

  SetBus (bus) ;

  if (channel <= NoOfChannels)
	Channel = (unsigned char)(channel-1) ;
  else
	// Use 0 as default
	Channel = 0 ;

  if ((Value >= 0)&&(Value < 128)) {
	(*MidiMessage2)[0] = (unsigned char)(EventType + Channel) ;
	(*MidiMessage2)[1] = (unsigned char)Value ;
	MidiOut->sendMessage( MidiMessage2 );
    Record (MidiMessage2) ;
  }
}


void MidiOutput::Event (int bus, unsigned int EventType, unsigned int channel, unsigned int Lsb, unsigned int Msb) {
  unsigned char Channel ;


  SetBus (bus) ;

  if (channel <= NoOfChannels)
	Channel = (unsigned char)(channel-1) ;
  else
	// Use 0 as default
	Channel = 0 ;

  (*MidiMessage3)[0] = (unsigned char)(EventType + Channel) ;
  (*MidiMessage3)[1] = (unsigned char)Lsb ;
  (*MidiMessage3)[2] = (unsigned char)Msb ;

  //cout << "MidiOutput::Event() " << hex << (int)((*MidiMessage3)[0]) << " " ;
  //cout << hex << (int)((*MidiMessage3)[1]) << " " ;
  //cout << hex << (int)((*MidiMessage3)[2]) << endl ;
  //cout << dec << endl ;

  MidiOut->sendMessage( MidiMessage3 );
  Record (MidiMessage3) ;
}


void MidiOutput::PlayNote (unsigned int Value, unsigned int Duration) {
  unsigned char Channel ;
  Channel = (unsigned char)(this->Channel) ;

  SetBus( 0 );

  // Note On: 144, Value, 90
  if ((Value >= 0)&&(Value < 128)) {
	(*MidiMessage3)[0] = (unsigned char)(144 + Channel) ;
	(*MidiMessage3)[1] = (unsigned char)Value ;
	(*MidiMessage3)[2] = 127 ;
	MidiOut->sendMessage( MidiMessage3 );
  }

  // Pause while the note sounds
  //MilliSleep (Duration) ;
  this_thread::sleep_for (chrono::milliseconds((Duration * 60000) / Tempo)) ;


  // Note Off: 128, Value, 90
  if ((Value >= 0)&&(Value < 128)) {
	(*MidiMessage3)[0] = (unsigned char)(128 + Channel) ;
	(*MidiMessage3)[1] = (unsigned char)Value ;
	(*MidiMessage3)[2] = 127 ;
	MidiOut->sendMessage( MidiMessage3 );
  }
}

void MidiOutput::SetChannel (unsigned int Value) {
  if (Value < NoOfChannels)
	Channel = Value ;
  else
	// Use 0 as default
	Channel = 0 ;
}

void MidiOutput::SetBus (int Value) {
  if (Value < NoOfBusses)
	Bus = Value ;
  else
	// Use 0 as default
	Bus = 0 ;
	MidiOut->selectVirtualPort (Bus);
}

MidiOutput::MidiOutput (string ClientName, string PortName, int NoOfPorts) {
  Recording = false ;
  Tempo = 1 ;
  this->ClientName = ClientName ;
  this->PortName = PortName ;
  MidiMessage2 = new std::vector<unsigned char> (2, 0) ;
  MidiMessage3 = new std::vector<unsigned char> (3, 0) ;

  // Create Midi Client
  //MidiClient = NewMidiClient (ClientName) ;
  try {
    MidiOut = new RtMidiOut(ClientName);
  }
  catch ( RtError &error ) {
    error.printMessage();
    exit( -1 );
  }

  // Create Midi Ports
  if (Alsa) {
	stringstream FullPortName ;

	for (int i=0; i<NoOfPorts; i++) {
	  FullPortName.clear () ;
	  FullPortName.str ("") ;
	  FullPortName << PortName << "_" << i ;
	  try {
		MidiOut->openVirtualPort (FullPortName.str()) ;
	  }
	  catch ( RtError &error ) {
		error.printMessage();
		exit( -1 );
	  }
	}

	// Default to Port 0
	try {
	  SetBus (0) ;
	}
	catch ( RtError &error ) {
	  error.printMessage();
	  exit( -1 );
	}
  }
  else { // Not Alsa
	unsigned int i = 0, nPorts = MidiOut->getPortCount();
	if ( nPorts == 0 ) {
	  cout << "No output ports available!" << endl;
	  return ;
	}

	if ( nPorts == 1 ) {
	  cout << "\nOpening " << MidiOut->getPortName() << endl;
	}
	else {
	  for ( i=0; i<nPorts; i++ ) {
		PortName = MidiOut->getPortName(i);
		cout << "  Output port #" << i << ": " << PortName << '\n';
	  }

	  do {
		cout << "\nChoose a port number: ";
		cin >> i;
	  } while ( i >= nPorts );
	}

	cout << "\n";
	MidiOut->openPort( i );
  }

  // Default to channel 0
  Channel = 0 ;
}


MidiInput::MidiInput (string ClientName, string PortName, RtMidiIn::RtMidiCallback Callback, void *This) {
  this->ClientName = ClientName ;
  this->PortName = PortName ;

  // Create Midi Client
  //MidiClient = NewMidiClient (ClientName) ;
  try {
    MidiIn = new RtMidiIn(ClientName);
  }
  catch ( RtError &error ) {
    error.printMessage();
    exit( -1 );
  }

  // Create Midi Port
  if (Alsa)
	MidiIn->openVirtualPort (PortName) ;
  else {
	unsigned int i = 0, nPorts = MidiIn->getPortCount();
	if ( nPorts == 0 ) {
	  cout << "No input ports available!" << endl;
	  return ;
	}

	if ( nPorts == 1 ) {
	  cout << "\nOpening " << MidiIn->getPortName() << endl;
	}
	else {
	  for ( i=0; i<nPorts; i++ ) {
		PortName = MidiIn->getPortName(i);
		cout << "  Input port #" << i << ": " << PortName << '\n';
	  }

	  do {
		cout << "\nChoose a port number: ";
		cin >> i;
	  } while ( i >= nPorts );
	}

	cout << "\n";
	MidiIn->openPort( i );
  }
  MidiIn->setCallback(Callback, This) ;
  MidiIn->ignoreTypes(false, false, false) ;
}
