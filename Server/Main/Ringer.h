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

#ifndef __Ringer__
#define __Ringer__

#include "Composition.h"
#include "Method.h"
#include "tinythread.h"

using namespace std ;
using namespace tthread ;


/**
 * @brief Creates and controls the ringing thread.
 *
 */
class Ringer {
private:
  thread *Thread ; /**< The execution thread for the ringer */
  Composition *Composition1 ; /**< A pointer to the composition object */
  Method *CurrentMethod ; /**< A pointer to the object that controls the current method */
  bool  Ringing ; /**< Set true if ringing should commence */
  bool  Alive ; /**< Set false if server should die. */

public:

  /**
   * @brief Constructor
   *
   * @param CompositionPath The absolute path to the composition file.
   */
  Ringer (string CompositionPath) ;
  
  /**
   * @brief The thread entry point.
   *
   *
   */
 // void operator() (void) ;
  
  /**
   * @brief The thread entry point.
   *
   *
   */
  void EntryPoint (void) ;
  
  /**
   * @brief Creates a new thread
   *
   */
  void Start (void) ;
  
  /**
   * @brief Wait for the thread to terminate
   *
   */
  void Join (void) ;
  
  /**
   * @brief Sets the Ringing flag true so the thread will commence ringing
   *
   */
  void Ring (void) ;
  
  /**
   * @brief Tells the Method object to perform a bob
   *
   */
  void Bob (void) ;
  
  /**
   * @brief Tells the Method object to perform a single
   *
   */
  void Single (void) ;
  
  /**
   * @brief Tells the Method object to perform a plain
   *
   */
  void Plain (void) ;
  
  /**
   * @brief Sets the Ringing flag false so the thread will stop ringing
   *
   */
  void Abort (void) ;
  
  /**
   * @brief Sets the Alive flag false so the thread will terminate.
   *
   */
  void Kill (void) ;
  
  /**
   * @brief Tells the Method object to set a new note length.
   *
   * @param NoteLength The note length.
   */
  void SetNoteLength (int NoteLength) ;
  
  /**
   * @brief Tells the Method object to set a new midi channel.
   *
   * @param MidiChannel The midi channel.
   */
  void SetMidiChannel (int MidiChannel) ;

} ;

#endif
