//
//  Name:  DateTime.h
//
//  Description:  This class is an interface between the
//                c++ ctime class and the user.
//
//  Interface:
//
//

#ifndef DATETIME_H
#define DATETIME_H
#include <ctime>

class DateTime {
 public:

  DateTime();
  ~DateTime();
  const char* getLocalTime() const;
  void setStartTime();
  float getRelativeTime() const;

 private:
  clock_t _startTime;   // used in conjunction with getRelativeTime()
};
  
#endif
