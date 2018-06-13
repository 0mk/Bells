// Implementation of the ClientSocket class

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

#include "ClientSocket.h"
#include "SocketException.h"



ClientSocket::ClientSocket ( std::string host, int port ) {
  if ( ! Socket::create() ) {
	throw SocketException ( "Could not create client socket." );
  }
  if ( ! Socket::connect ( host, port ) ) {
	throw SocketException ( "Could not bind to port." );
  }
}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const {
  if ( ! Socket::send ( s ) ) {
	throw SocketException ( "Could not write to socket." );
  }
  return *this;
}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const {
  if ( ! Socket::recv ( s ) ) {
	throw SocketException ( "Could not read from socket." );
  }
  return *this;
}
