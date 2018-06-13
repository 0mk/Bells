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

#ifndef __CompositionData__
#define __CompositionData__

#include <iostream>
#include <fstream>
#include <list>
#include <set>

using namespace std ;


/**
 * @brief Data for one method from the composition file is parsed into this class.
 * @brief The data can be read from the class during ringing via the Get... methods.
 *
 */
class MethodData {
public:

  /**
   * @brief Represents a row of place notation.
   *
   */
  typedef set<int> PlaceRow ;

  /**
   * @brief Constructor
   *
   */
  MethodData (void) ;

  /**
   * @brief Sets the name of the method.
   *
   * @param Name The name of the method.
   */
  void SetMethodName (const char *Name) ;

  /**
   * @brief Sets the number of bells.
   *
   * @param Number The number of bells.
   */
  void SetNoOfBells (const char *Number) ;

  /**
   * @brief Gets the number of bells.
   *
   * @return int The number of bells.
   */
  int GetNoOfBells (void) ;

  /**
   * @brief Gets the entire place notation for the half-lead.
   *
   * @return list<PlaceRow> The place notation.
   */
  list<PlaceRow> *GetPn (void) ;

  /**
   * @brief Gets the place notation for a plain lead-end.
   *
   * @return PlaceRow * The place notation.
   */
  PlaceRow *GetPlainLE (void) ;

  /**
   * @brief Gets the place notation for a bob lead-end.
   *
   * @return PlaceRow * The place notation.
   */
  PlaceRow *GetBobLE (void) ;

  /**
   * @brief Gets the place notation for a single lead-end.
   *
   * @return PlaceRow * The place notation.
   */
  PlaceRow *GetSingleLE (void) ;

  /**
   * @brief Adds a place to the current place-notation.
   *
   * @param Place The place to add.
   */
  void AddPlace (const char *Place) ;

  /**
   * @brief Creates a new empty row of place notation.
   *
   */
  void NewPlaceRow (void) ;

  /**
   * @brief Adds a row of place notation to the list held in PlaceRows.
   *
   */
  void AddPlaceRow (void) ;

  /**
   * @brief Sets the place notation of PlainLE
   *
   */
  void AddPlainLE (void) ;

  /**
   * @brief Sets the place notation of BobLE
   *
   */
  void AddBobLE (void) ;

  /**
   * @brief Sets the place notation of SingleLE
   *
   */
  void AddSingleLE (void) ;

  /**
   * @brief Prints the entire place notation to standard output.
   *
   */
  void ListPn (void) ;

private:
  string MethodName ; /**< The method name */
  int NoOfBells ; /**< The number of bells */
  list<PlaceRow> PlaceRows ; /**< A list of all place notation rows in the half-lead */
  PlaceRow *PnRow ; /**< A pointer to the current place row - used only for parsing */
  PlaceRow PlainLE ; /**< Holds the plain lead-end place notation */
  PlaceRow BobLE ; /**< Holds the bob lead-end place notation */
  PlaceRow SingleLE ; /**< Holds the single lead-end place notation */
} ;


/**
 * @brief Method data from the composition file is parsed into this class.
 *
 * The method data can be read from or written to the class via the CurrentMethod method.
 *
 * It is intended to add a search facility to this class to allow for
 * ringing spliced compositions \n
 * but in this version of the server, only one method is supported.
 *
 *
 */
class CompositionData {
public:

  /**
   * @brief Creates a new MethodData object and places it on the list.
   * @brief Sets CurrentMethodData to point to the object.
   *
   */
  void AddMethod (void) ;

  /**
   * @brief Returns a pointer to the current method data.
   *
   * @return MethodData * The current method data.
   *
   */
  MethodData *CurrentMethod(void) ;

private:
  list<MethodData> MethodList ; /**< A list of MethodData objects */
  MethodData *CurrentMethodData ; /**< Points to the head of the list */
} ;

#endif
