#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

using LinuxParser::MemInfo;

// DEBUG - REMOVE
#include<iostream>

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
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
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

MemInfo hashIt(const string &input) {
  if(input == LinuxParser::kMemTotal) return MemInfo::kMemTotal_;
  if(input == LinuxParser::kMemFree) return MemInfo::kMemFree_;
  if(input == LinuxParser::kMemBuffers) return MemInfo::kMemBuffers_;
  if(input == LinuxParser::kMemCached) return MemInfo::kMemCached_;
  if(input == LinuxParser::kMemSReclaimable) return MemInfo::kMemSReclaimable_;
  if(input == LinuxParser::kMemShmem) return MemInfo::kMemShmem_;
  return MemInfo::kNone_;
}

void SetMemInfoFromKey(const string &key, const string &value, std::map<int, float> &mapInfo) {
  switch(hashIt(key)) {
    case LinuxParser::MemInfo::kMemTotal_: mapInfo[MemInfo::kMemTotal_] = std::stof(value); break;
    case LinuxParser::MemInfo::kMemFree_: mapInfo[MemInfo::kMemFree_] = std::stof(value); break;
    case LinuxParser::MemInfo::kMemBuffers_: mapInfo[MemInfo::kMemBuffers_] = std::stof(value); break;
    case LinuxParser::MemInfo::kMemCached_: mapInfo[MemInfo::kMemCached_] = std::stof(value); break;
    case LinuxParser::MemInfo::kMemSReclaimable_: mapInfo[MemInfo::kMemSReclaimable_] = std::stof(value); break;
    case LinuxParser::MemInfo::kMemShmem_: mapInfo[MemInfo::kMemShmem_] = std::stof(value); break;
    case LinuxParser::MemInfo::kNone_: return;
    default: return;
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
  string kbText;

  std::map<int, float> memInfo;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      // populate the map
      while (linestream >> key >> value) {
        SetMemInfoFromKey(key, value, memInfo);
      }
      
      if(memInfo.size() == 6){
        break;
      }
    }
  }

  float memCached = memInfo[MemInfo::kMemCached_] + memInfo[MemInfo::kMemSReclaimable_] - memInfo[MemInfo::kMemShmem_];
  float memUtil = (memInfo[MemInfo::kMemTotal_] - memInfo[MemInfo::kMemFree_] - (
    memInfo[MemInfo::kMemBuffers_] + memCached
  )) / memInfo[MemInfo::kMemTotal_];

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
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
