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

#ifndef __Midi__
#define __Midi__

#include <string>
#include <cstdlib>
#include <fstream>
#include "RtMidi.h"

#if defined(__WINDOWS_MM__)
  #define Alsa false
#else // Unix variants
  #define Alsa true
  //#include <alsa/asoundlib.h>
#endif

//#include "../Main/tinythread.h"


using namespace std ;
//using namespace tthread ;

/**
 * @brief Handles midi output.
 *
 */

class MidiOutput {
private:
  bool Recording ;
  int Tempo ;
  static const unsigned int NoOfChannels=16 ; /**< The number of midi channels */
  static const int NoOfBusses=16 ; /**< The number of midi busses */
  RtMidiOut *MidiOut ;
  RtMidiOut *MidiOut2 ;
  int MidiPort ; /**< Midi port number */
  int Channel ; /**< Midi channel */
  int Bus ; /**< Midi Bus */
  string ClientName ; /**< Name of midi client */
  string PortName ; /**< Name of midi port */
  std::vector<unsigned char> *MidiMessage2 ;
  std::vector<unsigned char> *MidiMessage3 ;
  string OutputFilePath ;
  string InputFilePath ;
  ofstream OutputFile ;
  ifstream InputFile ;


public:

  /**
   * @brief Constructor
   *
   * @param ClientName The client name
   * @param PortName The port base name
   * @param NoOfPorts The number of ports to create
   */
  MidiOutput (string ClientName, string PortName, int NoOfPorts) ;

  /**
   * @brief Sets the tempo.
   *
   * @param Tempo The tempo.
   */
  void SetTempo (int Tempo) ;

  /**
   * @brief Plays a midi note.
   *
   * @param Value The note value.
   * @param Duration The note length.
   */
  void PlayNote (unsigned int Value, unsigned int Duration) ;

  void NoteOn (int bus, unsigned int Channel, unsigned int Value, unsigned int Velocity) ;
  void NoteOff (int bus, unsigned int Channel, unsigned int Value, unsigned int Velocity) ;
  void Event (int bus, unsigned int EventType, unsigned int Channel, unsigned int Value) ;
  void Event (int bus, unsigned int EventType, unsigned int Channel, unsigned int Lsb, unsigned int Msb) ;
  void NotesOff (void) ;
  void NotesOff (int bus, int Channel) ;
  void Record (std::vector<unsigned char> *MidiMessage) ;
  void RecordWait (int Time) ;
  bool OpenOutputFile (string FilePath) ;
  void CloseOutputFile (void) ;

  /**
   * @brief Sets the midi channel.
   *
   * @param Value The midi channel.
   */
  void SetChannel (unsigned int Value) ;
  void SetBus (int Value) ;

} ;



/**
 * @brief Handles midi input.
 *
 */

class MidiInput {
private:
  RtMidiIn *MidiIn ;
  string ClientName ; /**< Name of midi client */
  string PortName ; /**< Name of midi port */


public:

  /**
   * @brief Constructor
   *
   * @param ClientName The client name
   * @param PortName The port name
   * @param Callback The function to handle midi input
   * @param This this pointer of calling object
   */
  MidiInput (string ClientName, string PortName, RtMidiIn::RtMidiCallback Callback, void *This) ;

} ;

#endif
