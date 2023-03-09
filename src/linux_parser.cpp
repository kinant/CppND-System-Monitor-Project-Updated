#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

using LinuxParser::CPUStates;
using LinuxParser::MemInfo;
using LinuxParser::ProcessTime;

// DEBUG - REMOVE
#include <iostream>
using std::cout;
using std::endl;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

MemInfo LinuxParser::HashIt(const string &input) {
  if (input == LinuxParser::kMemTotal) return MemInfo::kMemTotal_;
  if (input == LinuxParser::kMemFree) return MemInfo::kMemFree_;
  if (input == LinuxParser::kMemBuffers) return MemInfo::kMemBuffers_;
  if (input == LinuxParser::kMemCached) return MemInfo::kMemCached_;
  if (input == LinuxParser::kMemSReclaimable) return MemInfo::kMemSReclaimable_;
  if (input == LinuxParser::kMemShmem) return MemInfo::kMemShmem_;
  return MemInfo::kNone_;
}

void LinuxParser::SetMemInfoFromKey(const string &key, const string &value,
                                    std::map<int, float> &mapInfo) {
  switch (HashIt(key)) {
    case LinuxParser::MemInfo::kMemTotal_:
      mapInfo[MemInfo::kMemTotal_] = std::stof(value);
      break;
    case LinuxParser::MemInfo::kMemFree_:
      mapInfo[MemInfo::kMemFree_] = std::stof(value);
      break;
    case LinuxParser::MemInfo::kMemBuffers_:
      mapInfo[MemInfo::kMemBuffers_] = std::stof(value);
      break;
    case LinuxParser::MemInfo::kMemCached_:
      mapInfo[MemInfo::kMemCached_] = std::stof(value);
      break;
    case LinuxParser::MemInfo::kMemSReclaimable_:
      mapInfo[MemInfo::kMemSReclaimable_] = std::stof(value);
      break;
    case LinuxParser::MemInfo::kMemShmem_:
      mapInfo[MemInfo::kMemShmem_] = std::stof(value);
      break;
    case LinuxParser::MemInfo::kNone_:
      return;
    default:
      return;
  }
}

// TODO: Read and return the system memory utilization
// As per: https://knowledge.udacity.com/questions/658540
// MemCached = Cached + SReclaimable - Shmem
// (MemTotal - MemFree - (Buffers + MemCached))/MemTotal
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;

  std::map<int, float> memInfo;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      // populate the map
      if (linestream >> key >> value) {
        SetMemInfoFromKey(key, value, memInfo);
      }

      if (memInfo.size() == 6) {
        break;
      }
    }
  }

  float memCached = memInfo[MemInfo::kMemCached_] +
                    memInfo[MemInfo::kMemSReclaimable_] -
                    memInfo[MemInfo::kMemShmem_];
  float memUtil = (memInfo[MemInfo::kMemTotal_] - memInfo[MemInfo::kMemFree_] -
                   (memInfo[MemInfo::kMemBuffers_] + memCached)) /
                  memInfo[MemInfo::kMemTotal_];

  return memUtil;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string s_uptime;
  string line;
  long uptime = 0;

  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);

    std::istringstream linestream(line);
    linestream >> s_uptime;

    uptime = std::stod(s_uptime);
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = CpuUtilization();

  long activeJiffies =
      (stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
       stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
       stol(jiffies[CPUStates::kSoftIRQ_]) + stol(jiffies[CPUStates::kSteal_]));

  return activeJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();

  long idleJiffies =
      (stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]));

  return idleJiffies;
}

// TODO: Read and return CPU utilization
// https://knowledge.udacity.com/questions/925549
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string value;

  vector<string> values{};

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      if (linestream >> key) {
        if (key == kProcCpu) {
          while (linestream >> value) {
            values.push_back(value);
          }
        }
      }
    }
  }
  return values;
}

int LinuxParser::GetSystemProcessInfo(const string &procKey) {
  string key;
  string value;
  string line;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      // populate the map
      if (linestream >> key >> value) {
        if (key == procKey) {
          return std::stoi(value);
        } else if (key == procKey) {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return GetSystemProcessInfo(kProcesses); }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return GetSystemProcessInfo(kProcsRunning);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (filestream.is_open()) {
    if (getline(filestream, line)) {
      return line;
    }
  }
  return "";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string key;
  string value;
  string line;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      if (linestream >> key >> value) {
        if (key == kProcessRam) {
          long mb = KB_TO_MB(stol(value));
          return to_string(mb);
        }
      }
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      if (linestream >> key >> value) {
        // cout << "LINE :" << line << endl;
        // cout << "   key: " << key << endl;
        // cout << "   value: " << value << endl;

        if (key == kProcessUid) {
          return value;
        }
      }
    }
  }
  return "";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string user;
  string sPid;
  char x;
  string line;
  string uid = Uid(pid);

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if (linestream >> user >> x >> sPid) {
        if (Uid(pid) == sPid) {
          return user;
        }
      }
    }
  }
  return "";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
long LinuxParser::UpTime(int pid) {
  string value;
  string line;

  int counter = 0;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> value) {
        if (counter == ProcessTime::kStartTime) {
          // Wasn't updating, read this:
          // https://knowledge.udacity.com/questions/756714
          return (UpTime()) - (stol(value) / HERTZ);
        }

        counter++;
      }
    }
  }
  return 0;
}

float LinuxParser::CpuUtilization(int pid) {
  string line;
  string key;
  string value;

  float utime{0};
  float stime{0};
  float cutime{0};
  float cstime{0};
  float starttime{0};

  int counter = 0;

  // cout << endl;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> value) {
        switch (counter) {
          case ProcessTime::kUTime:
            utime = stof(value);
            break;
          case ProcessTime::kSTime:
            stime = stof(value);
            break;
          case ProcessTime::kCUTime:
            cutime = stof(value);
            break;
          case ProcessTime::kCSTime:
            cstime = stof(value);
            break;
          case ProcessTime::kStartTime:
            starttime = stof(value);
            break;
        }
        counter++;
      }
    }
  }

  //cout << endl;
  //cout << "pid: " << pid << endl;
  //cout << "utime: " << utime << endl;
  //cout << "stime: " << stime << endl;
  //cout << "cutime: " << cutime << endl;
  //cout << "cstime: " << cstime << endl;
  //cout << "starttime: " << starttime << endl;

  float uptime = UpTime();
  float totaltime_ticks = utime + stime + cutime + cstime;
  float elapsedtime_ticks = (uptime * HERTZ) - starttime;


  float cpu_usage = ((float) totaltime_ticks / float(elapsedtime_ticks));

  // cout << endl;
  // cout << "uptime: " << uptime << endl;
  // cout << "totaltime: " << totaltime << endl;
  // cout << "seconds: " << seconds << endl;
  // cout << "cpu_usage: " << cpu_usage << endl;

  return cpu_usage;
}
