#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

bool System::IsExistingProcess(int pid) {
  for(Process &p : processes_) {
    if(p.Pid() == pid) {
      return true;
    }
  }
  return false;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  // clear processes
  processes_.clear();

  // get all process ids
  vector<int> pids = LinuxParser::Pids();

  // create processes and add them to the vector
  for(int pid : pids) {
    Process process = Process(pid);
    processes_.emplace_back(process);
  }

  // sort the vector
  

  return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
  return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
  return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime();
}