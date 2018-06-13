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

#ifndef __Method__
#define __Method__

#include <vector>
#include "CompositionData.h"
#include "Midi.h"


/**
 * @brief This class represents and controls the ringing of a method.
 *
 */
class Method {
private:

  /**
   * @brief Possible values of ringing stroke.
   *
   */
  enum stroke {
	handstroke, backstroke
  } ;

  /**
   * @brief Possible types of lead-end
   *
   */
  enum leadend {
	plain, bob, single
  } ;

  int NrBells ; /**< The number of bells */
  int NoteLength ; /**< The duration of each bell sound */
  int HandstrokeDelay ; /**< Length of post-handstroke pause */
  int BackstrokeDelay ; /**< Length of post-backstroke pause */
  int Delay ; /**< Length of pause after current stroke  */
  int NoOfRounds ; /**< The number of rounds to ring at the end of the method */
  int Tenor ; /**< The midi note value of the tenor bell */
  bool Aborted ; /**< Set true to abort the method */
  stroke Stroke ; /**< The current stroke */
  leadend Leadend ; /**< The type of the next lead-end */
  vector<int> *Rounds ; /**< Pointer to the bell sequence for Rounds */
  vector<int> *MidiSequence ; /**< Pointer to a sequence of midi notes */
  // We need two BellSequences 
  // We apply a place-notation transformation to one to generate the other
  // Then they swap roles as progress is made through the place notation
  vector<int> *BellSequence1 ; /**< Pointer to a bell-number sequence */
  vector<int> *BellSequence2 ; /**< Pointer to a bell-number sequence */
  // Two double pointers are used as an efficient way to swap the sequences
  vector<int> **ThisSequence ; /**< Double pointer to the current bell-number sequence */
  vector<int> **NextSequence ; /**< Double pointer to the next bell-number sequence */
  list<MethodData::PlaceRow> * Pn ; /**< Pointer to the half-lead place notation */
  list<MethodData::PlaceRow>::iterator PnFit ; /**< Forward iterator for the half-lead place notation */
  list<MethodData::PlaceRow>::reverse_iterator PnRit ; /**< Reverse iterator for the half-lead place notation */
  MethodData::PlaceRow * PlainLE ; /**< Pointer to the plain lead-end place notation */
  MethodData::PlaceRow * BobLE ; /**< Pointer to the bob lead-end place notation */
  MethodData::PlaceRow * SingleLE ; /**< Pointer to the single lead-end place notation */
  MidiOutput *MidiPortal ; /**< Pointer to the midi-interface object */

  /**
   * @brief Converts a number between zero and six to a note on the major scale.
   *
   * @param Note The number to convert.
   *
   */
  int Major (int Number) ;

public:

  /**
   * @brief Constructor
   *
   */
  Method (void) ;

  /**
   * @brief Controls the ringing of the method.
   *
   */
  void Ring (void) ;

  /**
   * @brief Sets the next lead-end to be a bob.
   *
   */
  void Bob (void) ;

  /**
   * @brief Sets the next lead-end to be a single.
   *
   */
  void Single (void) ;

  /**
   * @brief Sets the next lead-end to be a plain.
   *
   */
  void Plain (void) ;

  /**
   * @brief Sets the Aborted flag to true to cause the method to abort.
   *
   */
  void Abort (void) ;

  /**
   * @brief Prepares to start the method from scratch.
   *
   */
  void Reset (void) ;

  /**
   * @brief Determines whether the method has come round.
   *
   * @return bool True if it has.
   */
  bool IsRounds (void) ;

  /**
   * @brief Play the next sequence of bells.
   *
   */
  void PlaySequence (void) ;

  /**
   * @brief Repeat the current sequence
   *
   * @param NoOfTimes Number of repetitions
   */
  void RepeatSequence (int NoOfTimes) ;

  /**
   * @brief Play a single bell on the midi device.
   *
   * @param Note The note to play.
   */
  void PlayNote (int Note) ;

  /**
   * @brief Calculate the next bell sequence to ring by applying a row of place notation to the current sequence.
   *
   * @param Start The start of the row of place notation to apply.
   * @param End The end of the row of place notation to apply.
   */
  void GetNextSequence (MethodData::PlaceRow::iterator Start, MethodData::PlaceRow::iterator End) ;

  /**
   * @brief Set the number of bells and place notation from the MethodData \n
   * Allocate initial Bell and midi sequences \n
   * Calculate delays and set stroke to handstroke.
   *
   * @param MethodData A pointer to the MethodData object.
   */
  void Read (MethodData *MethodData) ;

  /**
   * @brief Set the duration of a midi note
   *
   * @param NoteLength The note duration
   */
  void SetNoteLength (int NoteLength) ;

  /**
   * @brief Set the handstroke and backstroke delays.
   *
   *
   */
  void SetDelays (void) ;

  /**
   * @brief Set the midi channel to use.
   *
   * @param MidiChannel The midi channel
   */
  void SetMidiChannel (int MidiChannel) ;

  /**
   * @brief Create a new alsa midi client.
   *
   * @return snd_seq_t * A handle on the midi client.
   */
  //snd_seq_t *OpenMidiClient (void) ;

  /**
   * @brief Create a new alsa midi port.
   *
   * @param handle The midi client handle
   * @return int The midi port.
   */
  //int NewMidiPort (snd_seq_t *handle) ;
} ;

#endif
