#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  unsigned long prevIdle_{0};
  unsigned long prevActive_{0};
  unsigned long prevTotal_{0};
};

#endif