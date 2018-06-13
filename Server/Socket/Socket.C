// Implementation of the Socket class.

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                  Copyright (c) <2002> by <Rob Tougher>.                     //
//                  Win32 code added by Martin Kohn <2011>                     //
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


#include "Socket.h"
#include <string.h>
#include <sstream>
#include <errno.h>
#include <fcntl.h>
#include <iostream>


using namespace std ;

#ifdef WIN32

bool SocketsInitialised = false ;
struct addrinfo *result = NULL, *ptr = NULL, hints;
WSADATA wsaData ;

#define socklen_t int
#define MSG_NOSIGNAL 0
#define EAFNOSUPPORT WSAEAFNOSUPPORT 

void InitialiseSockets (void) {
  cout << "InitialiseSockets" << endl ;
  WSAStartup(MAKEWORD(2,2), &wsaData) ;
}

void CleanupSockets (void) {
  cout << "CleanupSockets" << endl ;
  WSACleanup () ;
}

#else

void InitialiseSockets (void) {
}

void CleanupSockets (void) {
}

#endif

Socket::Socket() : m_sock ( -1 ) {
  #ifdef WIN32
  ZeroMemory( &hints, sizeof(hints) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  #endif
  memset ( &m_addr, 0, sizeof ( m_addr ) );
}

Socket::~Socket() {
  if ( is_valid() ) {
    //::close ( m_sock );
    closesocket ( m_sock );
  }
}

bool Socket::create() {
  m_sock = socket ( AF_INET, SOCK_STREAM, 0 );
  if ( ! is_valid() )
    return false;

  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;

  return true;
}



bool Socket::bind ( const int port ) {
  if ( ! is_valid() ) {
	return false;
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock, ( struct sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( bind_return == -1 ) {
	return false;
  }
  return true;
}


bool Socket::listen() const {
  if ( ! is_valid() ) {
	return false;
  }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );

  if ( listen_return == -1 ) {
	return false;
  }
  return true;
}


bool Socket::accept ( Socket& new_socket ) const {
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 ) {
	#ifdef WIN32
	cout << "accept failed with error: " <<  WSAGetLastError() << endl ;
	#endif
    return false;
  }
  else
    return true;
}


bool Socket::send ( const string s ) const {
  int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  if ( status == -1 ) {
	return false;
  }
  else {
	return true;
  }
}


int Socket::recv ( string& s ) const {
  char buf [ MAXRECV + 1 ];

  s = "";

  memset ( buf, 0, MAXRECV + 1 );

  int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( status == -1 ) {
	#ifdef WIN32
	cout << "status = -1   Error = " << WSAGetLastError() << "  in Socket::recv" << endl ;
	#else
	cout << "status = -1   errno = " << errno << "  in Socket::recv" << endl ;
	#endif
	return 0;
  }
  else if ( status == 0 ) {
	return 0;
  }
  else {
	s = buf;
	return status;
  }
}



bool Socket::connect ( const string host, const int port ) {
  if ( ! is_valid() ) return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status ;
  #ifdef WIN32

  string Port ;
  stringstream S ;
  S << port ;
  Port = S.str() ;
  status = getaddrinfo(host.c_str(), Port.c_str(), &hints, &result);
  if ( status != 0 ) {
	cout << "getaddrinfo failed with error: " << status << endl ;
	CleanupSockets() ;
	return false;
  }

  for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
	// Create a SOCKET for connecting to server
	m_sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (m_sock == INVALID_SOCKET) {
	  cout << "socket failed with error: " << WSAGetLastError() << endl ;
	  CleanupSockets() ;
	  return false;
	}

	// Connect to server.
	status = ::connect( m_sock, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (status == SOCKET_ERROR) {
	  closesocket(m_sock);
	  m_sock = INVALID_SOCKET;
	  continue;
	}
	break;
  }

  freeaddrinfo(result);

  if (m_sock == INVALID_SOCKET) {
	cout << "Unable to connect to server!" << endl ;
	CleanupSockets() ;
	return false ;
  }
  else
	return true ;

  #else

  status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );
  if ( errno == EAFNOSUPPORT ) return false;
  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );
  if ( status == 0 )
    return true;
  else
    return false;

  #endif
}

void Socket::set_non_blocking ( const bool b ) {
  #ifdef WIN32
  u_long Arg = 0 ;
  if (b) Arg = 1 ;
  ioctlsocket (m_sock, FIONBIO, &Arg) ;
  #else
  int opts;
  opts = fcntl ( m_sock, F_GETFL );

  if ( opts < 0 ) {
	return;
  }
  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock, F_SETFL,opts );
  #endif
}
