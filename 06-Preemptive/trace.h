#ifndef TRACE_H
#define TRACE_H

void trace_context_switch(int end);
void trace_task_info(size_t task_idx);
int trace_init(void);
void trace_exit(void);

#endif
