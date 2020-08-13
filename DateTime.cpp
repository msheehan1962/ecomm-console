//
//  Name:  DateTime.h
//
//  Description:  This class is an interface between the
//                c++ ctime class and the user.
//
//  Interface:
//
//

#include "DateTime.h"
//#include <ctime>

DateTime::DateTime() {}

DateTime::~DateTime() {}

const char* DateTime::getLocalTime() const {
  time_t rawTime;
  time(&rawTime);
  return ctime(&rawTime);
}

void DateTime::setStartTime() {
  _startTime = clock();  
}

float DateTime::getRelativeTime() const {
  clock_t time;
  time = clock() - _startTime;
  return (((float)time)/CLOCKS_PER_SEC);
}

#ifdef STANDALONE_OBJECT
#include <iostream>
using namespace std;

int main()
{
  DateTime dt;

  cout << "Current: " << dt.getLocalTime() << endl;

  dt.setStartTime();
  for (int i{0}; i < 9000000; i++)
    dt.getLocalTime();

  cout << dt.getRelativeTime() << "seconds\n";

    return 0;

}
#endif
