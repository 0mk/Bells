#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <iostream>
#include <sstream>

using namespace std ;


enum loglevel_e
  {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

class logIt {
public:
  logIt(loglevel_e _loglevel = logERROR) {
	//_buffer << _loglevel << " :" << string( _loglevel > logDEBUG ? (_loglevel - logDEBUG) * 4 : 1 , ' ');
	_buffer << string( _loglevel > logDEBUG ? (_loglevel - logDEBUG) * 4 : 0 , ' ');
  }

  template <typename T>
  logIt & operator<<(T const & value) {
	_buffer << value;
	return *this;
  }

  ~logIt() {
	_buffer << endl;
	// This is atomic according to the POSIX standard
	// http://www.gnu.org/s/libc/manual/html_node/Streams-and-Threads.html
	cerr << _buffer.str();
  }

private:
  ostringstream _buffer;
};

extern loglevel_e loglevel;


#ifdef __Release__
#define Debug(level) false && cout
#else
#define Debug(level) \
if (level > loglevel) ; \
else logIt(level)
#endif // __Release__

#define dout Debug(logDEBUG)

#endif // _LOGGER_HPP_
