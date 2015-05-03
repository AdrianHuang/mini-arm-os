#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "io.h"

#define LOG_FILE "tool/log"

#ifdef GET_CXT_SWITCH_COST
extern int get_current_task(void);
static int fd;

static unsigned int get_reload()
{
	return *(unsigned int *) 0xE000E014;
}

static unsigned int get_current()
{
	return *(unsigned int *) 0xE000E018;
}

void trace_context_switch(int end)
{
	static unsigned int prev_tick, tick_count;
	static size_t prev_task;

	char buf[128];
	int len;

	/*
	 * The SVC and SysTick handler share the same handler, so we
	 * need to make sure that the SysTick is configured and starts to
	 * count.
	 *
	 * Context switch call path:
	 *	1) systick_handler (restore user state and load kernel state)
	 *    	2) choose the next task (while(1) loop of the "main" function)
	 *	3) activate function (restore kernel state and load user state)
	 *
	 * That's why this function is invoked in the beginning of the
	 * systick_handler and in the end of the activate function.
	 */
	if (!get_current())
		return ;

	if (!end) {
		prev_task = get_current_task();
		prev_tick = get_current();
		tick_count++;
		return ;
	}

	len = snprintf(buf, 128, "switch %d %d %d %d %d %d\n",
			prev_task, get_current_task(), tick_count,
			get_reload(), prev_tick, get_current());

	write(fd, buf, len);
}

void trace_task_info(size_t task_idx)
{
	char buf[128];
	int len = snprintf(buf, 128, "task %d 0 Task %d\n", task_idx, task_idx);
	write(fd, buf, len);
}

int trace_init(void)
{
	fd = open(LOG_FILE, O_CREAT | O_RDWR);

	return fd;
}

void trace_exit(void)
{
	close(fd);
}

#else /* GET_CXT_SWITCH_COST is not enabled */

inline void trace_task_info(size_t task_idx)
{
}

inline int trace_init(void)
{
	return 0;
}

inline void trace_exit(void)
{
}

#endif /* GET_CXT_SWITCH_COST */
