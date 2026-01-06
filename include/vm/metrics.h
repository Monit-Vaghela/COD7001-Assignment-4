#ifndef METRICS_H
#define METRICS_H

#include <time.h>

// Instruction count
extern long instruction_count;

// Time spent performing differnt things
extern clock_t stack_time;
extern clock_t memory_time;
extern clock_t alu_time;
extern clock_t control_time;
extern clock_t others;

// Total execution time
extern clock_t exec_start;
extern clock_t exec_end;

#endif
