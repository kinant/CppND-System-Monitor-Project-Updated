#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int f_hours;
  int f_minutes;
  int f_seconds;

  // calculate minutes, hours and seconds
  f_minutes = (seconds / 60) % 60;
  f_hours = (seconds / 60) / 60;
  f_seconds = seconds % 60;

  // blank string to populate
  string sElapsedTime = "";

  // we add a 0 if hours < 10
  if (f_hours < 10) {
    sElapsedTime.push_back('0');
  }

  sElapsedTime += std::to_string(f_hours);
  sElapsedTime.push_back(':');

  // add a 0 if minutes < 10
  if (f_minutes < 10) {
    sElapsedTime.push_back('0');
  }

  sElapsedTime += std::to_string(f_minutes);
  sElapsedTime.push_back(':');

  // add a 0 if seconds < 10
  if (f_seconds < 10) {
    sElapsedTime.push_back('0');
  }

  sElapsedTime += std::to_string(f_seconds);

  return sElapsedTime;
}