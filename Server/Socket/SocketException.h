// SocketException class

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                  Copyright (c) <2002> by <Rob Tougher>.                     //
//                                                                             //
//                     This file is part of libSocket.                         //
//                                                                             //
//      libSocket is free software: you can redistribute it and/or modify it   //
//      under the terms of the GNU General Public License as published by      //
//      the Free Software Foundation, either version 3 of the License, or      //
//      (at your option) any later version.                                    //
//                                                                             //
//      libSocket is distributed in the hope that it will be useful, but       //
//      WITHOUT ANY WARRANTY; without even the implied warranty of             //
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//      GNU General Public License for more details.                           //
//                                                                             //
//      You should have received a copy of the GNU General Public License      //
//      along with libSocket.  If not, see <http://www.gnu.org/licenses/>.     //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////



#ifndef SocketException_class
#define SocketException_class

#include <string>

class SocketException
{
 public:
  SocketException ( std::string s ) : m_s ( s ) {};
  ~SocketException (){};

  std::string description() { return m_s; }

 private:

  std::string m_s;

};

#endif
