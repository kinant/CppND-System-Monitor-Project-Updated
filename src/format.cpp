#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int f_hours;
  int f_minutes;
  int f_seconds;

  f_minutes = (seconds / 60) % 60;
  f_hours = (seconds / 60) / 60;
  f_seconds = seconds % 60;

  string sElapsedTime = "";

  if (f_hours < 10) {
    sElapsedTime.push_back('0');
  }

  sElapsedTime += std::to_string(f_hours);
  sElapsedTime.push_back(':');

  if (f_minutes < 10) {
    sElapsedTime.push_back('0');
  }

  sElapsedTime += std::to_string(f_minutes);
  sElapsedTime.push_back(':');

  if (f_seconds < 10) {
    sElapsedTime.push_back('0');
  }

  sElapsedTime += std::to_string(f_seconds);

  return sElapsedTime;
}