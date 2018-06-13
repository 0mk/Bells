// Definition of the Socket class

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


#ifndef Socket_class
#define Socket_class


#ifdef WIN32

  #define WIN32_LEAN_AND_MEAN
  #define WINVER 0x600

  #include <windows.h>
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <stdlib.h>
  #include <stdio.h>


  // Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
  #pragma comment (lib, "Ws2_32.lib")
  #pragma comment (lib, "Mswsock.lib")
  #pragma comment (lib, "AdvApi32.lib")

#else

  #include <sys/types.h>
  #include <sys/socket.h>
//  #include <netinet/in.h>
//  #include <netdb.h>
  #include <arpa/inet.h>
  #define closesocket close

#endif

#include <unistd.h>
#include <string>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string ) const;
  int recv ( std::string& ) const;


  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }

 private:

  int m_sock;
  sockaddr_in m_addr;


};

extern void InitialiseSockets(void) ;
extern void CleanupSockets(void) ;


#endif
