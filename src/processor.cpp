#include "processor.h"

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
// Implementation of:
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() {
  float newIdle = LinuxParser::IdleJiffies();
  float newActive = LinuxParser::ActiveJiffies();
  float newTotal = LinuxParser::Jiffies();

  float totalD = newTotal - prevTotal_;
  float idleD = newIdle - prevIdle_;

  float cpuPercentage = (totalD - idleD) / totalD;

  prevIdle_ = newIdle;
  prevActive_ = newActive;
  prevTotal_ = newTotal;

  return cpuPercentage;
}