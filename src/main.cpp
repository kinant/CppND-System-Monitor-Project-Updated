#include "ncurses_display.h"
#include "system.h"

// FOR DEBUGGING - DELETE LATER
#include <chrono>
#include <iostream>
#include <thread>

#include "format.h"

void PrintDebug(System &system, bool runForever = false) {
  int n = 1;
  std::vector<Process> processes;
  // NCursesDisplay::Display(system);
  while (true) {
    std::cout << "System Metrics" << std::endl;
    std::cout << "CPU utilization " << system.Cpu().Utilization() * 100
              << std::endl;
    std::cout << "Memory utilization " << system.MemoryUtilization() * 100
              << std::endl;
    std::cout << "Total Processes " << system.TotalProcesses() << std::endl;
    std::cout << "Running Processes " << system.RunningProcesses() << std::endl;
    std::cout << "System Uptime " << Format::ElapsedTime(system.UpTime())
              << std::endl;
    std::cout << " " << std::endl;
    processes = system.Processes();
    std::cout << "Processes: " << std::endl;
    std::cout << " " << std::endl;

    for (int i = 0; i < n; ++i) {
      std::cout << processes[i].Command() << std::endl;
      std::cout << "Process ID " << processes[i].Pid() << std::endl;
      std::cout << "User " << processes[i].User() << std::endl;
      std::cout << "CPU utilization " << processes[i].CpuUtilization() * 100
                << std::endl;
      std::cout << "Memory utilization " << processes[i].Ram() << std::endl;
      std::cout << "Process Uptime "
                << Format::ElapsedTime(processes[i].UpTime()) << std::endl;
      std::cout << " " << std::endl;
    }
    std::this_thread::sleep_for(
        std::chrono::seconds(1));  // wait for one second, as otherwise
                                   // printouts will be coming in too fast

    if (!runForever) {
      break;
    }
  }
}

int main() {
  System system;

  //PrintDebug(system);

  NCursesDisplay::Display(system);
}