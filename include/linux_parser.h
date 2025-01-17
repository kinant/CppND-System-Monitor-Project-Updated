#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

// for conversions
#define KB_TO_MB(kb) (kb / 1024)
#define HERTZ sysconf(_SC_CLK_TCK)

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// For memory/mem info
const std::string kMemTotal{"MemTotal"};
const std::string kMemFree{"MemFree"};
const std::string kMemBuffers{"Buffers"};
const std::string kMemCached{"Cached"};
const std::string kMemSReclaimable{"SReclaimable"};
const std::string kMemShmem{"Shmem"};

// for processes
const std::string kProcesses{"processes"};
const std::string kProcsRunning{"procs_running"};

const std::string kProcCpu{"cpu"};

// for process
const std::string kProcessUid("Uid");
const std::string kProcessRam("VmSize");

// So we can use a switch statement and map for MemInfo
// https://stackoverflow.com/questions/650162/why-cant-the-switch-statement-be-applied-to-strings
enum MemInfo {
  kMemTotal_,
  kMemFree_,
  kMemBuffers_,
  kMemCached_,
  kMemSReclaimable_,
  kMemShmem_,
  kNone_
};

MemInfo HashIt(const std::string &input);
void SetMemInfoFromKey(const std::string &key, const std::string &value,
                       std::map<int, float> &mapInfo);

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int GetSystemProcessorInfo(const std::string &procKey);
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

// Process
enum ProcessTime {
  kUTime = 13,
  kSTime = 14,
  kCUTime = 15,
  kCSTime = 16,
  kStartTime = 21,
};

std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long UpTime(int pid);
float CpuUtilization(int pid);
};  // namespace LinuxParser

#endif